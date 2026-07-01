#include "astra_device_serial.h"

AstraResult astra_device_serial_validate(const AstraDeviceSerial* serial) {
    if(!serial) {
        return astra_result_error(AstraStatusInvalidArgument, "serial is null");
    }

    if(serial->baud_rate == 0) {
        return astra_result_error(AstraStatusInvalidArgument, "serial baud rate is invalid");
    }

    return astra_result_ok();
}
