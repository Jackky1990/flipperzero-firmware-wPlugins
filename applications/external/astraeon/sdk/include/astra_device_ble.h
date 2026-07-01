#pragma once

#include <stdbool.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool advertising;
    bool connected;
} AstraDeviceBLEStatus;

AstraResult astra_device_ble_status_validate(const AstraDeviceBLEStatus* status);

#ifdef __cplusplus
}
#endif
