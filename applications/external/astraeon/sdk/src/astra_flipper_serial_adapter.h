#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "astra_device_serial.h"
#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    AstraFlipperSerialChannelPrimary = 0,
    AstraFlipperSerialChannelSecondary,
    AstraFlipperSerialChannelCount,
} AstraFlipperSerialChannelId;

#define ASTRA_FLIPPER_SERIAL_RX_BUFFER_SIZE (256u)

typedef struct {
    AstraFlipperSerialChannelId id;
    AstraDeviceSerialConfig contract;
    void* flipper_handle;
    uint8_t rx_buffer[ASTRA_FLIPPER_SERIAL_RX_BUFFER_SIZE];
    volatile size_t rx_head;
    volatile size_t rx_tail;
    volatile size_t rx_count;
    volatile size_t rx_bytes_received;
    volatile size_t rx_bytes_read;
    volatile size_t rx_overflow_count;
    volatile size_t rx_error_count;
    volatile bool rx_active;
    bool acquired;
} AstraFlipperSerialChannelBinding;

typedef struct {
    AstraFlipperSerialChannelBinding channels[AstraFlipperSerialChannelCount];
    size_t channel_count;
} AstraFlipperSerialAdapter;

AstraResult astra_flipper_serial_adapter_init(AstraFlipperSerialAdapter* adapter);

AstraResult astra_flipper_serial_adapter_validate(const AstraFlipperSerialAdapter* adapter);

size_t astra_flipper_serial_adapter_channel_count(const AstraFlipperSerialAdapter* adapter);

const AstraFlipperSerialChannelBinding* astra_flipper_serial_adapter_channel_at(
    const AstraFlipperSerialAdapter* adapter,
    size_t index);

AstraResult astra_flipper_serial_adapter_is_baud_supported(uint32_t baud_rate);

AstraResult astra_flipper_serial_adapter_acquire(
    AstraFlipperSerialAdapter* adapter,
    const AstraDeviceSerialConfig* config);

AstraResult astra_flipper_serial_adapter_release(
    AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel);

AstraResult astra_flipper_serial_adapter_is_busy(
    const AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel,
    bool* out_busy);

AstraResult astra_flipper_serial_adapter_write(
    AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel,
    const uint8_t* data,
    size_t length,
    size_t* out_written);

AstraResult astra_flipper_serial_adapter_start_async_rx(
    AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel);

AstraResult astra_flipper_serial_adapter_stop_async_rx(
    AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel);

AstraResult astra_flipper_serial_adapter_rx_available(
    const AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel,
    size_t* out_available);

AstraResult astra_flipper_serial_adapter_read(
    AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel,
    uint8_t* out_data,
    size_t length,
    size_t* out_read);

AstraResult astra_flipper_serial_adapter_clear(
    AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel);

AstraResult astra_flipper_serial_adapter_rx_isr_copy_byte(
    AstraFlipperSerialAdapter* adapter,
    AstraFlipperSerialChannelId channel,
    uint8_t byte);

#ifdef __cplusplus
}
#endif
