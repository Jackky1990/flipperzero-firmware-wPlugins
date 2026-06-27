#pragma once

#include "astra_event.h"
#include "astra_result.h"
#include "astra_runtime_context.h"

#ifdef __cplusplus
extern "C" {
#endif

AstraResult astra_dispatcher_init(void);
AstraResult astra_dispatcher_dispatch(AstraEvent* event);

AstraResult astra_dispatcher_init_context(
    AstraRuntimeContext* context);

AstraResult astra_dispatcher_dispatch_context(
    AstraRuntimeContext* context,
    AstraEvent* event);

#ifdef __cplusplus
}
#endif
