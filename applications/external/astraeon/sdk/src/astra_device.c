#include "astra_device.h"

#include <string.h>

AstraResult astra_device_init(AstraDevice* device, const char* id) {
    if(!device || !id || id[0] == '\0') {
        return astra_result_error(AstraStatusInvalidArgument, "device init invalid");
    }

    size_t id_length = strlen(id);
    if(id_length >= ASTRA_ID_MAX_LEN) {
        return astra_result_error(AstraStatusInvalidArgument, "device id too long");
    }

    memset(device, 0, sizeof(*device));
    memcpy(device->id, id, id_length + 1);

    return astra_device_capabilities_init(&device->capabilities);
}

AstraResult astra_device_validate(const AstraDevice* device) {
    if(!device || device->id[0] == '\0') {
        return astra_result_error(AstraStatusInvalidArgument, "device is invalid");
    }

    return astra_device_capabilities_validate(&device->capabilities);
}

AstraResult astra_device_set_capabilities(
    AstraDevice* device,
    const AstraDeviceCapabilities* capabilities) {
    if(!device || !capabilities) {
        return astra_result_error(AstraStatusInvalidArgument, "device capabilities set invalid");
    }

    AstraResult result = astra_device_capabilities_validate(capabilities);
    if(result.status != AstraStatusOk) {
        return result;
    }

    device->capabilities = *capabilities;
    return astra_result_ok();
}

const AstraDeviceCapabilities* astra_device_capabilities(const AstraDevice* device) {
    return device ? &device->capabilities : 0;
}
