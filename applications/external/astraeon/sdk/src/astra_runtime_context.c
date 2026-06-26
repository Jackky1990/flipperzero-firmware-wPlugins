#include "astra_runtime_context.h"

#include <string.h>

AstraResult astra_runtime_context_init(AstraRuntimeContext* context) {
    if(!context) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }

    memset(context, 0, sizeof(*context));

    context->initialized = true;

    return astra_result_ok();
}

AstraResult astra_runtime_context_reset(AstraRuntimeContext* context) {
    if(!context) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }

    memset(context, 0, sizeof(*context));

    return astra_result_ok();
}
