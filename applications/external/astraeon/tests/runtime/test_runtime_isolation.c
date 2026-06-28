#include "astra_tests.h"
#include "astra_runtime_manager.h"
#include "astra_registry.h"
#include "astra_event.h"

static AstraResult handler_a(AstraEvent* event) {
    (void)event;
    return astra_result_ok();
}

static AstraResult handler_b(AstraEvent* event) {
    (void)event;
    return astra_result_ok();
}

bool astra_test_runtime_isolation(void) {
    AstraRuntimeId id1 = 0;
    AstraRuntimeId id2 = 0;
    AstraRuntimeContext* ctx1 = 0;
    AstraRuntimeContext* ctx2 = 0;

    if(astra_runtime_manager_init().status != AstraStatusOk) return false;
    if(astra_runtime_create(&id1, &ctx1).status != AstraStatusOk) return false;
    if(astra_runtime_create(&id2, &ctx2).status != AstraStatusOk) return false;

    if(astra_registry_init_context(ctx1).status != AstraStatusOk) return false;
    if(astra_registry_init_context(ctx2).status != AstraStatusOk) return false;

    if(astra_registry_register_context(ctx1, AstraEventTypeStatus, handler_a).status != AstraStatusOk) return false;
    if(astra_registry_find_context(ctx1, AstraEventTypeStatus) != handler_a) return false;
    if(astra_registry_find_context(ctx2, AstraEventTypeStatus) != 0) return false;

    if(astra_registry_register_context(ctx2, AstraEventTypeStatus, handler_b).status != AstraStatusOk) return false;
    if(astra_registry_find_context(ctx2, AstraEventTypeStatus) != handler_b) return false;
    if(astra_registry_find_context(ctx1, AstraEventTypeStatus) != handler_a) return false;

    return id1 != id2;
}
