#include "astra_runtime_loop.h"
#include "astra_scheduler.h"
#include "astra_runtime_default.h"
#include <stdbool.h>

static bool astra_runtime_loop_initialized = false;

AstraResult astra_runtime_loop_init(void) {
    return astra_runtime_loop_init_context(astra_runtime_default_context());
}

AstraResult astra_runtime_loop_step(void) {
    return astra_runtime_loop_step_context(astra_runtime_default_context());
}

AstraResult astra_runtime_loop_shutdown(void) {
    return astra_runtime_loop_shutdown_context(astra_runtime_default_context());
}

AstraResult astra_runtime_loop_init_context(AstraRuntimeContext* context) {
    if(!context) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }

    AstraResult scheduler_result = astra_scheduler_init_context(context);
    if(scheduler_result.status != AstraStatusOk) {
        return scheduler_result;
    }

    astra_runtime_loop_initialized = true;
    return astra_result_ok();
}

AstraResult astra_runtime_loop_step_context(AstraRuntimeContext* context) {
    if(!context) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }

    if(!astra_runtime_loop_initialized) {
        return astra_result_error(AstraStatusInvalidArgument, "runtime loop is not initialized");
    }

    AstraResult result = astra_scheduler_step_context(context);
    if(result.status == AstraStatusNotFound) {
        return astra_result_ok();
    }

    return result;
}

AstraResult astra_runtime_loop_shutdown_context(AstraRuntimeContext* context) {
    if(!context) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }

    astra_runtime_loop_initialized = false;
    return astra_result_ok();
}
