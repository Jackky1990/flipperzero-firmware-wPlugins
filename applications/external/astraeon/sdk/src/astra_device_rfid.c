#include "astra_device_rfid.h"

AstraResult astra_device_rfid_validate(const AstraDeviceRFID* rfid) {
    if(!rfid) {
        return astra_result_error(AstraStatusInvalidArgument, "rfid is null");
    }

    return astra_result_ok();
}
