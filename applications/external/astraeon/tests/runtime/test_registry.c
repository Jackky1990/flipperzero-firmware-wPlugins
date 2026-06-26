#include "astra_tests.h"
#include "astra_registry.h"

static AstraResult astra_test_dummy_handler(AstraEvent* event) {
    (void)event;
    return astra_result_ok();
}

bool astra_test_registry(void) {
    if(astra_registry_init().status != AstraStatusOk) {
        return false;
    }

    if(astra_registry_register(AstraEventTypeStatus, astra_test_dummy_handler).status != AstraStatusOk) {
        return false;
    }

    return astra_registry_find(AstraEventTypeStatus) == 0;
}
