#include "astra_device_ble.h"

AstraResult astra_device_ble_status_validate(const AstraDeviceBLEStatus* status) {
    if(!status) {
        return astra_result_error(AstraStatusInvalidArgument, "ble status is null");
    }

    if(status->advertising && status->connected) {
        return astra_result_error(AstraStatusInvalidArgument, "ble status is invalid");
    }

    return astra_result_ok();
}
