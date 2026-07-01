#pragma once

#include <stdint.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t baud_rate;
} AstraDeviceSerial;

AstraResult astra_device_serial_validate(const AstraDeviceSerial* serial);

#ifdef __cplusplus
}
#endif
