#include "astra_flipper_serial_adapter.h"

#if defined(__has_include)
#if __has_include(<furi_hal_serial_control.h>) && __has_include(<furi_hal_serial_types.h>)
#define ASTRA_FLIPPER_SERIAL_HAS_CONTROL 1
#include <furi_hal_serial.h>
#include <furi_hal_serial_control.h>
#include <furi_hal_serial_types.h>
#endif
#endif

static AstraDeviceSerialConfig astra_flipper_serial_default_contract(
    AstraDeviceSerialChannel channel) {
    AstraDeviceSerialConfig config = {
        .channel = channel,
        .baud_rate = 115200,
        .data_bits = 8,
        .parity = AstraDeviceSerialParityNone,
        .stop_bits = AstraDeviceSerialStopBits1,
        .flow_control = AstraDeviceSerialFlowControlNone,
        .rx_buffer_size = 256,
        .tx_buffer_size = 256,
        .timeout_ms = 100,
    };
    return config;
}

static AstraResult astra_flipper_serial_adapter_resolve_channel(
    const AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel,
    const AstraFlipperSerialChannelBinding** out_binding) {
    if(!adapter || !out_binding) {
        return astra_result_error(
            AstraStatusInvalidArgument,
            "flipper serial resolve arguments invalid");
    }

    if(channel >= AstraFlipperSerialChannelCount) {
        return astra_result_error(
            AstraStatusInvalidArgument,
            "flipper serial channel is not allowlisted");
    }

    AstraResult result = astra_flipper_serial_adapter_validate(adapter);
    if(result.status != AstraStatusOk) {
        return result;
    }

    *out_binding = astra_flipper_serial_adapter_channel_at(adapter, (size_t)channel);
    if(!*out_binding) {
        return astra_result_error(AstraStatusNotFound, "flipper serial channel is unavailable");
    }

    return astra_result_ok();
}

static AstraResult astra_flipper_serial_adapter_resolve_channel_mut(
    AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel,
    AstraFlipperSerialChannelBinding** out_binding) {
    const AstraFlipperSerialChannelBinding* binding = 0;
    AstraResult result = astra_flipper_serial_adapter_resolve_channel(adapter, channel, &binding);
    if(result.status != AstraStatusOk) {
        return result;
    }

    *out_binding = &adapter->channels[(size_t)channel];
    return astra_result_ok();
}

static AstraFlipperSerialChannelId astra_flipper_serial_channel_from_contract(
    AstraDeviceSerialChannel channel) {
    switch(channel) {
    case AstraDeviceSerialChannelPrimary:
        return AstraFlipperSerialChannelPrimary;
    case AstraDeviceSerialChannelSecondary:
        return AstraFlipperSerialChannelSecondary;
    default:
        return AstraFlipperSerialChannelCount;
    }
}

static bool astra_flipper_serial_baud_allowlisted(uint32_t baud_rate) {
    static const uint32_t supported_baud_rates[] = {
        9600,
        19200,
        38400,
        57600,
        115200,
        230400,
        460800,
        921600,
    };

    for(size_t index = 0; index < sizeof(supported_baud_rates) / sizeof(supported_baud_rates[0]);
        ++index) {
        if(supported_baud_rates[index] == baud_rate) {
            return true;
        }
    }

    return false;
}

#if defined(ASTRA_FLIPPER_SERIAL_HAS_CONTROL)
static FuriHalSerialId astra_flipper_serial_control_id(AstraFlipperSerialChannelId channel) {
    static const FuriHalSerialId serial_ids[AstraFlipperSerialChannelCount] = {
        FuriHalSerialIdUsart,
        FuriHalSerialIdLpuart,
    };
    return serial_ids[channel];
}

static void astra_flipper_serial_adapter_on_rx(
    FuriHalSerialHandle* handle,
    FuriHalSerialRxEvent event,
    void* context);
#endif

static void astra_flipper_serial_binding_rx_clear(AstraFlipperSerialChannelBinding* binding) {
    if(binding) {
        binding->rx_head = 0;
        binding->rx_tail = 0;
        binding->rx_count = 0;
    }
}

static void astra_flipper_serial_binding_rx_reset(AstraFlipperSerialChannelBinding* binding) {
    if(binding) {
        astra_flipper_serial_binding_rx_clear(binding);
        binding->rx_bytes_received = 0;
        binding->rx_bytes_read = 0;
        binding->rx_overflow_count = 0;
        binding->rx_error_count = 0;
        binding->rx_active = false;
    }
}

static bool astra_flipper_serial_binding_rx_push_byte(
    AstraFlipperSerialChannelBinding* binding,
    uint8_t byte) {
    if(!binding) {
        return false;
    }

    if(binding->rx_count >= ASTRA_FLIPPER_SERIAL_RX_BUFFER_SIZE) {
        binding->rx_overflow_count += 1;
        return false;
    }

    binding->rx_buffer[binding->rx_head] = byte;
    binding->rx_head = (binding->rx_head + 1u) % ASTRA_FLIPPER_SERIAL_RX_BUFFER_SIZE;
    binding->rx_count += 1;
    binding->rx_bytes_received += 1;
    return true;
}

#if defined(ASTRA_FLIPPER_SERIAL_HAS_CONTROL)
static void astra_flipper_serial_adapter_on_rx(
    FuriHalSerialHandle* handle,
    FuriHalSerialRxEvent event,
    void* context) {
    AstraFlipperSerialChannelBinding* binding = context;
    if(!binding || !binding->rx_active) {
        return;
    }

    if(event & FuriHalSerialRxEventData) {
        while(furi_hal_serial_async_rx_available(handle)) {
            uint8_t byte = furi_hal_serial_async_rx(handle);
            astra_flipper_serial_binding_rx_push_byte(binding, byte);
        }
    }

    if(event & FuriHalSerialRxEventOverrunError) {
        binding->rx_overflow_count += 1;
        binding->rx_error_count += 1;
    }

    if(event & (FuriHalSerialRxEventFrameError | FuriHalSerialRxEventNoiseError)) {
        binding->rx_error_count += 1;
    }
}
#endif

AstraResult astra_flipper_serial_adapter_init(AstraFlipperSerialAdapter* adapter) {
    if(!adapter) {
        return astra_result_error(AstraStatusInvalidArgument, "flipper serial adapter is null");
    }

    adapter->channel_count = AstraFlipperSerialChannelCount;

    for(size_t index = 0; index < adapter->channel_count; ++index) {
        adapter->channels[index].id = (AstraFlipperSerialChannelId)index;
        adapter->channels[index].contract =
            astra_flipper_serial_default_contract((AstraDeviceSerialChannel)index);
        adapter->channels[index].flipper_handle = 0;
        astra_flipper_serial_binding_rx_reset(&adapter->channels[index]);
        adapter->channels[index].acquired = false;
    }

    return astra_result_ok();
}

AstraResult astra_flipper_serial_adapter_validate(const AstraFlipperSerialAdapter* adapter) {
    if(!adapter) {
        return astra_result_error(AstraStatusInvalidArgument, "flipper serial adapter is null");
    }

    if(adapter->channel_count != AstraFlipperSerialChannelCount) {
        return astra_result_error(
            AstraStatusInvalidArgument,
            "flipper serial channel count is invalid");
    }

    for(size_t index = 0; index < adapter->channel_count; ++index) {
        const AstraFlipperSerialChannelBinding* binding = &adapter->channels[index];
        if(binding->id != (AstraFlipperSerialChannelId)index) {
            return astra_result_error(
                AstraStatusInvalidArgument,
                "flipper serial channel id is invalid");
        }

        AstraResult result = astra_device_serial_config_validate(&binding->contract);
        if(result.status != AstraStatusOk) {
            return result;
        }
    }

    return astra_result_ok();
}

size_t astra_flipper_serial_adapter_channel_count(const AstraFlipperSerialAdapter* adapter) {
    if(!adapter) {
        return 0;
    }

    return adapter->channel_count;
}

const AstraFlipperSerialChannelBinding* astra_flipper_serial_adapter_channel_at(
    const AstraFlipperSerialAdapter* adapter,
    size_t index) {
    if(!adapter || index >= adapter->channel_count) {
        return 0;
    }

    return &adapter->channels[index];
}

AstraResult astra_flipper_serial_adapter_is_baud_supported(uint32_t baud_rate) {
    if(!astra_flipper_serial_baud_allowlisted(baud_rate)) {
        return astra_result_error(
            AstraStatusInvalidArgument,
            "flipper serial baud rate is unsupported");
    }

    return astra_result_ok();
}

AstraResult astra_flipper_serial_adapter_acquire(
    AstraFlipperSerialAdapter* adapter,
    const AstraDeviceSerialConfig* config) {
    if(!config) {
        return astra_result_error(AstraStatusInvalidArgument, "flipper serial config is null");
    }

    AstraResult result = astra_device_serial_config_validate(config);
    if(result.status != AstraStatusOk) {
        return result;
    }

    result = astra_flipper_serial_adapter_is_baud_supported(config->baud_rate);
    if(result.status != AstraStatusOk) {
        return result;
    }

    AstraFlipperSerialChannelId channel =
        astra_flipper_serial_channel_from_contract(config->channel);
    AstraFlipperSerialChannelBinding* binding = 0;
    result = astra_flipper_serial_adapter_resolve_channel_mut(adapter, channel, &binding);
    if(result.status != AstraStatusOk) {
        return result;
    }

    if(binding->acquired) {
        return astra_result_error(AstraStatusBusy, "flipper serial channel is already acquired");
    }

#if defined(ASTRA_FLIPPER_SERIAL_HAS_CONTROL)
    FuriHalSerialId serial_id = astra_flipper_serial_control_id(channel);
    if(furi_hal_serial_control_is_busy(serial_id)) {
        return astra_result_error(AstraStatusBusy, "flipper serial channel is busy");
    }

    FuriHalSerialHandle* handle = furi_hal_serial_control_acquire(serial_id);
    if(!handle) {
        return astra_result_error(AstraStatusBusy, "flipper serial channel acquire failed");
    }
    binding->flipper_handle = handle;
#else
    binding->flipper_handle = adapter;
#endif

    binding->contract = *config;
    binding->acquired = true;
    return astra_result_ok();
}

AstraResult astra_flipper_serial_adapter_release(
    AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel) {
    AstraFlipperSerialChannelBinding* binding = 0;
    AstraResult result =
        astra_flipper_serial_adapter_resolve_channel_mut(adapter, channel, &binding);
    if(result.status != AstraStatusOk) {
        return result;
    }

    if(!binding->acquired) {
        return astra_result_ok();
    }

#if defined(ASTRA_FLIPPER_SERIAL_HAS_CONTROL)
    if(binding->rx_active && binding->flipper_handle) {
        furi_hal_serial_async_rx_stop(binding->flipper_handle);
    }

    if(binding->flipper_handle) {
        furi_hal_serial_control_release(binding->flipper_handle);
    }
#endif

    binding->flipper_handle = 0;
    astra_flipper_serial_binding_rx_reset(binding);
    binding->acquired = false;
    return astra_result_ok();
}

AstraResult astra_flipper_serial_adapter_is_busy(
    const AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel,
    bool* out_busy) {
    const AstraFlipperSerialChannelBinding* binding = 0;
    if(!out_busy) {
        return astra_result_error(AstraStatusInvalidArgument, "flipper serial busy result is null");
    }

    *out_busy = false;

    AstraResult result = astra_flipper_serial_adapter_resolve_channel(adapter, channel, &binding);
    if(result.status != AstraStatusOk) {
        return result;
    }

    *out_busy = binding->acquired;

#if defined(ASTRA_FLIPPER_SERIAL_HAS_CONTROL)
    if(!*out_busy) {
        *out_busy = furi_hal_serial_control_is_busy(astra_flipper_serial_control_id(channel));
    }
#endif

    return astra_result_ok();
}

AstraResult astra_flipper_serial_adapter_write(
    AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel,
    const uint8_t* data,
    size_t length,
    size_t* out_written) {
    if(!out_written) {
        return astra_result_error(AstraStatusInvalidArgument, "flipper serial write result is null");
    }

    *out_written = 0;

    if(length > 0 && !data) {
        return astra_result_error(AstraStatusInvalidArgument, "flipper serial write data is null");
    }

    AstraFlipperSerialChannelBinding* binding = 0;
    AstraResult result = astra_flipper_serial_adapter_resolve_channel_mut(adapter, channel, &binding);
    if(result.status != AstraStatusOk) {
        return result;
    }

    if(!binding->acquired || !binding->flipper_handle) {
        return astra_result_error(
            AstraStatusPermissionDenied,
            "flipper serial channel is not acquired");
    }

    if(length == 0) {
        return astra_result_ok();
    }

#if defined(ASTRA_FLIPPER_SERIAL_HAS_CONTROL)
    furi_hal_serial_tx(binding->flipper_handle, data, length);
#endif

    *out_written = length;
    return astra_result_ok();
}

AstraResult astra_flipper_serial_adapter_start_async_rx(
    AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel) {
    AstraFlipperSerialChannelBinding* binding = 0;
    AstraResult result = astra_flipper_serial_adapter_resolve_channel_mut(adapter, channel, &binding);
    if(result.status != AstraStatusOk) {
        return result;
    }

    if(!binding->acquired || !binding->flipper_handle) {
        return astra_result_error(
            AstraStatusPermissionDenied,
            "flipper serial channel is not acquired");
    }

    if(binding->rx_active) {
        return astra_result_error(AstraStatusBusy, "flipper serial async rx is already active");
    }

    astra_flipper_serial_binding_rx_clear(binding);
    binding->rx_active = true;

#if defined(ASTRA_FLIPPER_SERIAL_HAS_CONTROL)
    furi_hal_serial_async_rx_start(
        binding->flipper_handle,
        astra_flipper_serial_adapter_on_rx,
        binding,
        true);
#endif

    return astra_result_ok();
}

AstraResult astra_flipper_serial_adapter_stop_async_rx(
    AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel) {
    AstraFlipperSerialChannelBinding* binding = 0;
    AstraResult result = astra_flipper_serial_adapter_resolve_channel_mut(adapter, channel, &binding);
    if(result.status != AstraStatusOk) {
        return result;
    }

    if(!binding->rx_active) {
        return astra_result_ok();
    }

#if defined(ASTRA_FLIPPER_SERIAL_HAS_CONTROL)
    if(binding->flipper_handle) {
        furi_hal_serial_async_rx_stop(binding->flipper_handle);
    }
#endif

    binding->rx_active = false;
    return astra_result_ok();
}

AstraResult astra_flipper_serial_adapter_rx_available(
    const AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel,
    size_t* out_available) {
    const AstraFlipperSerialChannelBinding* binding = 0;
    if(!out_available) {
        return astra_result_error(AstraStatusInvalidArgument, "flipper serial rx count is null");
    }

    *out_available = 0;

    AstraResult result = astra_flipper_serial_adapter_resolve_channel(adapter, channel, &binding);
    if(result.status != AstraStatusOk) {
        return result;
    }

    *out_available = binding->rx_count;
    return astra_result_ok();
}

AstraResult astra_flipper_serial_adapter_read(
    AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel,
    uint8_t* out_data,
    size_t length,
    size_t* out_read) {
    if(!out_read) {
        return astra_result_error(AstraStatusInvalidArgument, "flipper serial read count is null");
    }

    *out_read = 0;

    if(length > 0 && !out_data) {
        return astra_result_error(AstraStatusInvalidArgument, "flipper serial read buffer is null");
    }

    AstraFlipperSerialChannelBinding* binding = 0;
    AstraResult result = astra_flipper_serial_adapter_resolve_channel_mut(adapter, channel, &binding);
    if(result.status != AstraStatusOk) {
        return result;
    }

    while(*out_read < length && binding->rx_count > 0) {
        out_data[*out_read] = binding->rx_buffer[binding->rx_tail];
        binding->rx_tail = (binding->rx_tail + 1u) % ASTRA_FLIPPER_SERIAL_RX_BUFFER_SIZE;
        binding->rx_count -= 1;
        *out_read += 1;
    }

    binding->rx_bytes_read += *out_read;
    return astra_result_ok();
}

AstraResult astra_flipper_serial_adapter_clear(
    AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel) {
    AstraFlipperSerialChannelBinding* binding = 0;
    AstraResult result = astra_flipper_serial_adapter_resolve_channel_mut(adapter, channel, &binding);
    if(result.status != AstraStatusOk) {
        return result;
    }

    astra_flipper_serial_binding_rx_clear(binding);
    return astra_result_ok();
}

AstraResult astra_flipper_serial_adapter_rx_isr_copy_byte(
    AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel,
    uint8_t byte) {
    AstraFlipperSerialChannelBinding* binding = 0;
    AstraResult result = astra_flipper_serial_adapter_resolve_channel_mut(adapter, channel, &binding);
    if(result.status != AstraStatusOk) {
        return result;
    }

    if(!binding->rx_active) {
        return astra_result_error(
            AstraStatusPermissionDenied,
            "flipper serial async rx is not active");
    }

    astra_flipper_serial_binding_rx_push_byte(binding, byte);
    return astra_result_ok();
}
