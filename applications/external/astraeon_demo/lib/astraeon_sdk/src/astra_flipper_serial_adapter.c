#include "astra_flipper_serial_adapter.h"

#if defined(__has_include)
#if __has_include(<furi_hal_serial_control.h>) && __has_include(<furi_hal_serial_types.h>)
#define ASTRA_FLIPPER_SERIAL_HAS_CONTROL 1
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
    AstraResult result = astra_flipper_serial_adapter_resolve_channel_mut(adapter, channel, &binding);
    if(result.status != AstraStatusOk) {
        return result;
    }

    if(!binding->acquired) {
        return astra_result_ok();
    }

#if defined(ASTRA_FLIPPER_SERIAL_HAS_CONTROL)
    if(binding->flipper_handle) {
        furi_hal_serial_control_release(binding->flipper_handle);
    }
#endif

    binding->flipper_handle = 0;
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
