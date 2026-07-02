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

typedef struct {
    AstraFlipperSerialChannelId id;
    AstraDeviceSerialConfig contract;
    void* flipper_handle;
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

#ifdef __cplusplus
}
#endif
