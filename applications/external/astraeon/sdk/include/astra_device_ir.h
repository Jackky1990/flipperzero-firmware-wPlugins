#pragma once

#include <stdbool.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool supports_rx;
    bool supports_tx;
} AstraDeviceIR;

AstraResult astra_device_ir_validate(const AstraDeviceIR* ir);

#ifdef __cplusplus
}
#endif
