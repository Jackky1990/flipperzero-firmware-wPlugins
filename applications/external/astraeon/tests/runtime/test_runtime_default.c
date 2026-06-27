#include "astra_tests.h"
#include "astra_runtime_default.h"

bool astra_test_runtime_default(void) {
    AstraRuntimeContext* context = astra_runtime_default_context();

    if(!context) {
        return false;
    }

    if(astra_runtime_default_init().status != AstraStatusOk) {
        return false;
    }

    if(!context->initialized) {
        return false;
    }

    if(astra_runtime_default_reset().status != AstraStatusOk) {
        return false;
    }

    return !context->initialized;
}
