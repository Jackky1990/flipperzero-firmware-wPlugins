#pragma once

#include <stdbool.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool initialized;
    void* scheduler;
    void* registry;
    void* queue;
} AstraRuntimeContext;

AstraResult astra_runtime_context_init(AstraRuntimeContext* context);

AstraResult astra_runtime_context_reset(AstraRuntimeContext* context);

#ifdef __cplusplus
}
#endif
