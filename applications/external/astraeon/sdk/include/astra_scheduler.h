#pragma once

#include "astra_result.h"
#include "astra_event.h"
#include "astra_runtime_context.h"

#ifdef __cplusplus
extern "C" {
#endif

AstraResult astra_scheduler_init(void);
AstraResult astra_scheduler_schedule(const AstraEvent* event);
AstraResult astra_scheduler_step(void);

AstraResult astra_scheduler_init_context(AstraRuntimeContext* context);
AstraResult astra_scheduler_schedule_context(AstraRuntimeContext* context, const AstraEvent* event);
AstraResult astra_scheduler_step_context(AstraRuntimeContext* context);

#ifdef __cplusplus
}
#endif
