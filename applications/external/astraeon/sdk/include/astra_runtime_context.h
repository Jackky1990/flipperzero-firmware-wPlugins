#pragma once

#include <stdbool.h>

#include "astra_event_queue.h"
#include "astra_result.h"
#include "astra_service_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AstraRuntimeContext {
    bool initialized;
    bool loop_initialized;
    AstraEventQueue scheduler_queue;
    AstraServiceManager service_manager;
    void* scheduler;
    void* registry;
    void* queue;
} AstraRuntimeContext;

AstraResult astra_runtime_context_init(AstraRuntimeContext* context);
AstraResult astra_runtime_context_reset(AstraRuntimeContext* context);

AstraResult astra_runtime_context_set_scheduler(
    AstraRuntimeContext* context,
    void* scheduler);

void* astra_runtime_context_get_scheduler(
    AstraRuntimeContext* context);

AstraResult astra_runtime_context_set_registry(
    AstraRuntimeContext* context,
    void* registry);

void* astra_runtime_context_get_registry(
    AstraRuntimeContext* context);

#ifdef __cplusplus
}
#endif
