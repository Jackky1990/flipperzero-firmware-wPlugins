#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool supports_rx;
    bool supports_tx;
    uint32_t min_frequency_hz;
    uint32_t max_frequency_hz;
} AstraDeviceSubGhz;

AstraResult astra_device_subghz_validate(const AstraDeviceSubGhz* subghz);

#ifdef __cplusplus
}
#endif
