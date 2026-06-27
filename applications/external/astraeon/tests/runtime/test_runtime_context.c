#include <stddef.h>

#include "astra_tests.h"
#include "astra_runtime_context.h"

bool astra_test_runtime_context(void) {

    AstraRuntimeContext context;

    int scheduler;
    int registry;

    if(astra_runtime_context_init(NULL).status != AstraStatusInvalidArgument)
        return false;

    if(astra_runtime_context_init(&context).status != AstraStatusOk)
        return false;

    if(!context.initialized)
        return false;

    if(astra_runtime_context_set_scheduler(
        &context,
        &scheduler).status != AstraStatusOk)
        return false;

    if(astra_runtime_context_get_scheduler(&context)
        != &scheduler)
        return false;

    if(astra_runtime_context_set_registry(
        &context,
        &registry).status != AstraStatusOk)
        return false;

    if(astra_runtime_context_get_registry(&context)
        != &registry)
        return false;

    if(astra_runtime_context_reset(&context).status != AstraStatusOk)
        return false;

    if(context.initialized)
        return false;

    if(astra_runtime_context_get_scheduler(&context)!=NULL)
        return false;

    if(astra_runtime_context_get_registry(&context)!=NULL)
        return false;

    return true;
}
