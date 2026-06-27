#pragma once

#include "astra_result.h"
#include "astra_event.h"
#include "astra_runtime_context.h"

#ifdef __cplusplus
extern "C" {
#endif

AstraResult astra_event_bus_init(void);
AstraResult astra_event_bus_publish(AstraEvent* event);

AstraResult astra_event_bus_init_context(AstraRuntimeContext* context);
AstraResult astra_event_bus_publish_context(AstraRuntimeContext* context, AstraEvent* event);

#ifdef __cplusplus
}
#endif
