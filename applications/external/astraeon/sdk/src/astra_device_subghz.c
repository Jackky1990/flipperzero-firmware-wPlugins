#include "astra_device_subghz.h"

AstraResult astra_device_subghz_validate(const AstraDeviceSubGhz* subghz) {
    if(!subghz) {
        return astra_result_error(AstraStatusInvalidArgument, "subghz is null");
    }

    if(subghz->min_frequency_hz > subghz->max_frequency_hz) {
        return astra_result_error(AstraStatusInvalidArgument, "subghz frequency range is invalid");
    }

    return astra_result_ok();
}
