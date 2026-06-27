#include "astra_tests.h"
#include "astra_registry.h"
#include "astra_runtime_context.h"

static AstraResult astra_test_dummy_handler(AstraEvent* event) {
    (void)event;
    return astra_result_ok();
}

static AstraResult astra_test_replacement_handler(AstraEvent* event) {
    (void)event;
    return astra_result_ok();
}

bool astra_test_registry(void) {
    AstraRuntimeContext context;

    if(astra_registry_init().status != AstraStatusOk) {
        return false;
    }

    if(astra_registry_find(AstraEventTypeStatus) != 0) {
        return false;
    }

    if(astra_registry_register(AstraEventTypeStatus, astra_test_dummy_handler).status != AstraStatusOk) {
        return false;
    }

    if(astra_registry_find(AstraEventTypeStatus) != astra_test_dummy_handler) {
        return false;
    }

    if(astra_registry_register(AstraEventTypeStatus, astra_test_replacement_handler).status != AstraStatusOk) {
        return false;
    }

    if(astra_registry_find(AstraEventTypeStatus) != astra_test_replacement_handler) {
        return false;
    }

    if(astra_registry_register(AstraEventTypeStatus, 0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_context_init(&context).status != AstraStatusOk) {
        return false;
    }

    if(astra_registry_init_context(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_registry_init_context(&context).status != AstraStatusOk) {
        return false;
    }

    if(astra_registry_find_context(&context, AstraEventTypeStatus) != 0) {
        return false;
    }

    if(astra_registry_register_context(&context, AstraEventTypeStatus, astra_test_dummy_handler).status != AstraStatusOk) {
        return false;
    }

    if(astra_registry_find_context(&context, AstraEventTypeStatus) != astra_test_dummy_handler) {
        return false;
    }

    if(astra_registry_register_context(&context, AstraEventTypeStatus, 0).status != AstraStatusInvalidArgument) {
        return false;
    }

    return true;
}
