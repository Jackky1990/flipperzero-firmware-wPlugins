#pragma once

#include <stdbool.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool supports_read;
    bool supports_write;
    bool supports_emulate;
} AstraDeviceRFID;

AstraResult astra_device_rfid_validate(const AstraDeviceRFID* rfid);

#ifdef __cplusplus
}
#endif
