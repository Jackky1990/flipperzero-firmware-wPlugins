#pragma once

#include "astra_result.h"
#include "astra_event.h"
#include "astra_handler.h"
#include "astra_runtime_context.h"

#ifdef __cplusplus
extern "C" {
#endif

AstraResult astra_registry_init(void);
AstraResult astra_registry_register(AstraEventType type, AstraEventHandler handler);
AstraEventHandler astra_registry_find(AstraEventType type);

AstraResult astra_registry_init_context(AstraRuntimeContext* context);
AstraResult astra_registry_register_context(
    AstraRuntimeContext* context,
    AstraEventType type,
    AstraEventHandler handler);
AstraEventHandler astra_registry_find_context(
    AstraRuntimeContext* context,
    AstraEventType type);

#ifdef __cplusplus
}
#endif
