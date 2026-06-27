#pragma once

#include "astra_result.h"
#include "astra_runtime_context.h"

#ifdef __cplusplus
extern "C" {
#endif

AstraResult astra_runtime_loop_init(void);
AstraResult astra_runtime_loop_step(void);
AstraResult astra_runtime_loop_shutdown(void);

AstraResult astra_runtime_loop_init_context(
    AstraRuntimeContext* context);

AstraResult astra_runtime_loop_step_context(
    AstraRuntimeContext* context);

AstraResult astra_runtime_loop_shutdown_context(
    AstraRuntimeContext* context);

#ifdef __cplusplus
}
#endif
