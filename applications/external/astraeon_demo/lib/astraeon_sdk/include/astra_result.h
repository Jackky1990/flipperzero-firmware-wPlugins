#pragma once

#include <stdint.h>
#include "astra_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    AstraStatus status;
    const char* message;
    uint32_t code;
    void* context;
} AstraResult;

AstraResult astra_result_ok(void);
AstraResult astra_result_error(AstraStatus status, const char* message);
AstraResult astra_result_with_context(AstraStatus status, const char* message, uint32_t code, void* context);

#ifdef __cplusplus
}
#endif
