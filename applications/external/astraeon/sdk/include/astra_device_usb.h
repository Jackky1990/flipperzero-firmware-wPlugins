#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool supports_cdc;
    bool supports_hid;
    size_t max_packet_size;
} AstraDeviceUSB;

AstraResult astra_device_usb_validate(const AstraDeviceUSB* usb);

#ifdef __cplusplus
}
#endif
