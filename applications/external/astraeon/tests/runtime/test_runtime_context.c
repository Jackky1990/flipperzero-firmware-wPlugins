#include <stddef.h>
#include "astra_tests.h"

#include "astra_runtime_context.h"

bool astra_test_runtime_context(void) {

    AstraRuntimeContext context;

    if(astra_runtime_context_init(NULL).status != AstraStatusInvalidArgument)
        return false;

    if(astra_runtime_context_init(&context).status != AstraStatusOk)
        return false;

    if(!context.initialized)
        return false;

    if(astra_runtime_context_reset(&context).status != AstraStatusOk)
        return false;

    return context.initialized == false;
}
