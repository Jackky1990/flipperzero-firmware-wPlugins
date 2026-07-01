#include "astra_device_usb.h"

AstraResult astra_device_usb_validate(const AstraDeviceUSB* usb) {
    if(!usb) {
        return astra_result_error(AstraStatusInvalidArgument, "usb is null");
    }

    if((usb->supports_cdc || usb->supports_hid) && usb->max_packet_size == 0) {
        return astra_result_error(AstraStatusInvalidArgument, "usb packet size is invalid");
    }

    return astra_result_ok();
}
