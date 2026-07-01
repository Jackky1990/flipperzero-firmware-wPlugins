#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASTRA_DEVICE_CAPABILITY_NONE   0u
#define ASTRA_DEVICE_CAPABILITY_GPIO   (1u << 0)
#define ASTRA_DEVICE_CAPABILITY_USB    (1u << 1)
#define ASTRA_DEVICE_CAPABILITY_SERIAL (1u << 2)
#define ASTRA_DEVICE_CAPABILITY_BLE    (1u << 3)
#define ASTRA_DEVICE_CAPABILITY_NFC    (1u << 4)
#define ASTRA_DEVICE_CAPABILITY_RFID   (1u << 5)
#define ASTRA_DEVICE_CAPABILITY_IR     (1u << 6)
#define ASTRA_DEVICE_CAPABILITY_SUBGHZ (1u << 7)

typedef struct {
    uint32_t feature_flags;
} AstraDeviceCapabilities;

AstraResult astra_device_capabilities_init(AstraDeviceCapabilities* capabilities);

AstraResult astra_device_capabilities_validate(const AstraDeviceCapabilities* capabilities);

bool astra_device_capabilities_supports(
    const AstraDeviceCapabilities* capabilities,
    uint32_t feature);

#ifdef __cplusplus
}
#endif
