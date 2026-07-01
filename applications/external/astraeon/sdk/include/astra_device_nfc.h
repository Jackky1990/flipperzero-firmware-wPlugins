#pragma once

#include <stdbool.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool supports_poller;
    bool supports_listener;
} AstraDeviceNFC;

AstraResult astra_device_nfc_validate(const AstraDeviceNFC* nfc);

#ifdef __cplusplus
}
#endif
