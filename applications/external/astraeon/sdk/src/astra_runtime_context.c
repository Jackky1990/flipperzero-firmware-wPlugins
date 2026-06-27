#include "astra_runtime_context.h"

#include <string.h>

static AstraResult astra_runtime_context_validate(
    AstraRuntimeContext* context) {

    if(!context) {
        return astra_result_error(
            AstraStatusInvalidArgument,
            "context is null");
    }

    return astra_result_ok();
}

AstraResult astra_runtime_context_init(
    AstraRuntimeContext* context) {

    AstraResult r = astra_runtime_context_validate(context);
    if(r.status != AstraStatusOk)
        return r;

    memset(context,0,sizeof(*context));

    context->initialized = true;

    return astra_result_ok();
}

AstraResult astra_runtime_context_reset(
    AstraRuntimeContext* context) {

    AstraResult r = astra_runtime_context_validate(context);
    if(r.status != AstraStatusOk)
        return r;

    memset(context,0,sizeof(*context));

    return astra_result_ok();
}

AstraResult astra_runtime_context_set_scheduler(
    AstraRuntimeContext* context,
    void* scheduler) {

    AstraResult r = astra_runtime_context_validate(context);
    if(r.status != AstraStatusOk)
        return r;

    context->scheduler = scheduler;

    return astra_result_ok();
}

void* astra_runtime_context_get_scheduler(
    AstraRuntimeContext* context) {

    if(!context)
        return NULL;

    return context->scheduler;
}

AstraResult astra_runtime_context_set_registry(
    AstraRuntimeContext* context,
    void* registry) {

    AstraResult r = astra_runtime_context_validate(context);
    if(r.status != AstraStatusOk)
        return r;

    context->registry = registry;

    return astra_result_ok();
}

void* astra_runtime_context_get_registry(
    AstraRuntimeContext* context) {

    if(!context)
        return NULL;

    return context->registry;
}
