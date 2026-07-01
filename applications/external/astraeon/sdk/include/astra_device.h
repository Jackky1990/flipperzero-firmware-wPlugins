#pragma once

#include "astra_device_capabilities.h"
#include "astra_result.h"
#include "astra_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    AstraDeviceId id;
    AstraDeviceCapabilities capabilities;
} AstraDevice;

AstraResult astra_device_init(AstraDevice* device, const char* id);

AstraResult astra_device_validate(const AstraDevice* device);

AstraResult astra_device_set_capabilities(
    AstraDevice* device,
    const AstraDeviceCapabilities* capabilities);

const AstraDeviceCapabilities* astra_device_capabilities(const AstraDevice* device);

#ifdef __cplusplus
}
#endif
