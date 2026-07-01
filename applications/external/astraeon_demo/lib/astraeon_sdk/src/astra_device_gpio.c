#include "astra_device_gpio.h"

AstraResult astra_device_gpio_pin_validate(const AstraDeviceGPIOPin* pin) {
    if(!pin) {
        return astra_result_error(AstraStatusInvalidArgument, "gpio pin is null");
    }

    if(pin->mode > AstraDeviceGPIOModeInterrupt) {
        return astra_result_error(AstraStatusInvalidArgument, "gpio mode is invalid");
    }

    return astra_result_ok();
}
