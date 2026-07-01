#include "astra_device_nfc.h"

AstraResult astra_device_nfc_validate(const AstraDeviceNFC* nfc) {
    if(!nfc) {
        return astra_result_error(AstraStatusInvalidArgument, "nfc is null");
    }

    return astra_result_ok();
}
