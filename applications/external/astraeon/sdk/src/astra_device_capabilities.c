#include "astra_device_capabilities.h"

AstraResult astra_device_capabilities_init(AstraDeviceCapabilities* capabilities) {
    if(!capabilities) {
        return astra_result_error(AstraStatusInvalidArgument, "device capabilities is null");
    }

    capabilities->feature_flags = ASTRA_DEVICE_CAPABILITY_NONE;

    return astra_result_ok();
}

AstraResult astra_device_capabilities_validate(const AstraDeviceCapabilities* capabilities) {
    if(!capabilities) {
        return astra_result_error(AstraStatusInvalidArgument, "device capabilities is null");
    }

    return astra_result_ok();
}

bool astra_device_capabilities_supports(
    const AstraDeviceCapabilities* capabilities,
    uint32_t feature) {
    return capabilities && feature != ASTRA_DEVICE_CAPABILITY_NONE &&
           (capabilities->feature_flags & feature) == feature;
}
