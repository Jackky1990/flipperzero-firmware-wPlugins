#include <string.h>

#include "astra_service_manager.h"
#include "astra_runtime_context.h"
#include "astra_tests.h"

typedef struct {
    unsigned int starts;
    unsigned int stops;
    AstraServiceHealth health;
    bool fail_start;
    bool fail_stop;
} AstraTestServiceState;

static AstraResult astra_test_service_start(void* context) {
    AstraTestServiceState* state = context;
    state->starts++;
    if(state->fail_start) {
        return astra_result_error(AstraStatusError, "start failed");
    }
    return astra_result_ok();
}

static AstraResult astra_test_service_stop(void* context) {
    AstraTestServiceState* state = context;
    state->stops++;
    if(state->fail_stop) {
        return astra_result_error(AstraStatusError, "stop failed");
    }
    return astra_result_ok();
}

static AstraServiceHealth astra_test_service_health(void* context) {
    AstraTestServiceState* state = context;
    return state->health;
}

static AstraServiceDefinition astra_test_service_definition(
    const char* name,
    AstraTestServiceState* state) {
    AstraServiceDefinition definition = {
        .name = name,
        .start = astra_test_service_start,
        .stop = astra_test_service_stop,
        .health = astra_test_service_health,
        .context = state,
    };
    return definition;
}

bool astra_test_service_manager(void) {
    AstraRuntimeContext context_a;
    AstraRuntimeContext context_b;
    AstraTestServiceState state = {.health = AstraServiceHealthHealthy};
    AstraServiceDefinition definition = astra_test_service_definition("alpha", &state);
    AstraServiceHealth health = AstraServiceHealthUnknown;

    if(astra_service_manager_init_context(0).status != AstraStatusInvalidArgument) return false;
    if(astra_runtime_context_init(&context_a).status != AstraStatusOk) return false;
    if(astra_runtime_context_init(&context_b).status != AstraStatusOk) return false;

    if(astra_service_manager_register_context(0, &definition).status != AstraStatusInvalidArgument) return false;
    if(astra_service_manager_register_context(&context_a, 0).status != AstraStatusInvalidArgument) return false;
    if(astra_service_manager_register_context(&context_a, &definition).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&context_a, &definition).status != AstraStatusBusy) return false;
    if(astra_service_manager_find_context(&context_a, "alpha") == 0) return false;
    if(astra_service_manager_find_context(&context_b, "alpha") != 0) return false;
    if(astra_service_manager_find_context(0, "alpha") != 0) return false;
    if(astra_service_manager_start_context(0, "alpha").status != AstraStatusInvalidArgument) return false;
    if(astra_service_manager_stop_context(&context_a, 0).status != AstraStatusInvalidArgument) return false;

    if(astra_service_manager_health_context(&context_a, "alpha", &health).status != AstraStatusOk) return false;
    if(health != AstraServiceHealthStopped) return false;
    if(astra_service_manager_start_context(&context_a, "missing").status != AstraStatusNotFound) return false;

    state.fail_start = true;
    if(astra_service_manager_start_context(&context_a, "alpha").status != AstraStatusError) return false;
    if(astra_service_manager_find_context(&context_a, "alpha")->state != AstraServiceStateStopped) return false;
    state.fail_start = false;

    if(astra_service_manager_start_context(&context_a, "alpha").status != AstraStatusOk) return false;
    if(state.starts != 2) return false;
    if(astra_service_manager_start_context(&context_a, "alpha").status != AstraStatusBusy) return false;
    if(astra_service_manager_health_context(&context_a, "alpha", &health).status != AstraStatusOk) return false;
    if(health != AstraServiceHealthHealthy) return false;

    state.health = AstraServiceHealthDegraded;
    if(astra_service_manager_health_context(&context_a, "alpha", &health).status != AstraStatusOk) return false;
    if(health != AstraServiceHealthDegraded) return false;

    state.fail_stop = true;
    if(astra_service_manager_stop_context(&context_a, "alpha").status != AstraStatusError) return false;
    if(astra_service_manager_find_context(&context_a, "alpha")->state != AstraServiceStateRunning) return false;
    state.fail_stop = false;

    if(astra_service_manager_stop_context(&context_a, "alpha").status != AstraStatusOk) return false;
    if(state.stops != 2) return false;
    if(astra_service_manager_stop_context(&context_a, "alpha").status != AstraStatusBusy) return false;

    const char* names[ASTRA_SERVICE_MANAGER_MAX_SERVICES] = {
        "bravo", "charlie", "delta", "echo", "foxtrot", "golf", "hotel", "india",
    };
    for(size_t index = 0; index < ASTRA_SERVICE_MANAGER_MAX_SERVICES - 1; index++) {
        definition = astra_test_service_definition(names[index], &state);
        if(astra_service_manager_register_context(&context_a, &definition).status != AstraStatusOk) return false;
    }
    definition = astra_test_service_definition(names[ASTRA_SERVICE_MANAGER_MAX_SERVICES - 1], &state);
    if(astra_service_manager_register_context(&context_a, &definition).status != AstraStatusBusy) return false;

    if(astra_service_manager_init().status != AstraStatusOk) return false;
    definition = astra_test_service_definition("default", &state);
    if(astra_service_manager_register(&definition).status != AstraStatusOk) return false;
    if(astra_service_manager_find("default") == 0) return false;
    if(astra_service_manager_start("default").status != AstraStatusOk) return false;
    if(astra_service_manager_health("default", &health).status != AstraStatusOk) return false;
    if(astra_service_manager_stop("default").status != AstraStatusOk) return false;

    AstraServiceDefinition invalid = definition;
    invalid.name = "";
    if(astra_service_manager_register_context(&context_b, &invalid).status != AstraStatusInvalidArgument) return false;
    invalid = definition;
    invalid.start = 0;
    if(astra_service_manager_register_context(&context_b, &invalid).status != AstraStatusInvalidArgument) return false;
    invalid = definition;
    invalid.stop = 0;
    if(astra_service_manager_register_context(&context_b, &invalid).status != AstraStatusInvalidArgument) return false;
    if(astra_service_manager_health_context(&context_b, "missing", &health).status != AstraStatusNotFound) return false;
    if(astra_service_manager_health_context(&context_b, "missing", 0).status != AstraStatusInvalidArgument) return false;

    invalid = definition;
    invalid.name = "service-name-that-is-longer-than-thirty-one-characters";
    if(astra_service_manager_register_context(&context_b, &invalid).status != AstraStatusInvalidArgument) return false;

    invalid = astra_test_service_definition("no-health", &state);
    invalid.health = 0;
    if(astra_service_manager_register_context(&context_b, &invalid).status != AstraStatusOk) return false;
    if(astra_service_manager_start_context(&context_b, "no-health").status != AstraStatusOk) return false;
    if(astra_service_manager_health_context(&context_b, "no-health", &health).status != AstraStatusOk) return false;
    if(health != AstraServiceHealthUnknown) return false;
    if(astra_service_manager_stop_context(&context_b, "no-health").status != AstraStatusOk) return false;

    definition = astra_test_service_definition("invalid-health", &state);
    if(astra_service_manager_register_context(&context_b, &definition).status != AstraStatusOk) return false;
    if(astra_service_manager_start_context(&context_b, "invalid-health").status != AstraStatusOk) return false;
    state.health = (AstraServiceHealth)99;
    if(astra_service_manager_health_context(&context_b, "invalid-health", &health).status != AstraStatusInternalError) return false;

    if(astra_runtime_context_reset(&context_b).status != AstraStatusOk) return false;
    if(astra_service_manager_find_context(&context_b, "invalid-health") != 0) return false;

    return true;
}
