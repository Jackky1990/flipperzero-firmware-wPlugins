#include "astra_tests.h"
#include "astra_runtime_manager.h"

bool astra_test_runtime_manager(void) {
    AstraRuntimeId id = 0;
    AstraRuntimeContext* context = 0;

    if(astra_runtime_manager_init().status != AstraStatusOk) {
        return false;
    }

    if(!astra_runtime_default()) {
        return false;
    }

    if(!astra_runtime_get(0)) {
        return false;
    }

    if(astra_runtime_create(0, &context).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_create(&id, 0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_create(&id, &context).status != AstraStatusOk) {
        return false;
    }

    if(id == 0 || !context || !context->initialized) {
        return false;
    }

    if(astra_runtime_get(id) != context) {
        return false;
    }

    if(astra_runtime_destroy(id).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_get(id) != 0) {
        return false;
    }

    if(astra_runtime_destroy(id).status != AstraStatusNotFound) {
        return false;
    }

    if(astra_runtime_get(999) != 0) {
        return false;
    }

    if(astra_runtime_destroy(0).status != AstraStatusNotFound) {
        return false;
    }

    return true;
}
