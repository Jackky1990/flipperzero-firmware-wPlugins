#include <string.h>

#include "astra_service_manager.h"
#include "astra_runtime_context.h"
#include "astra_tests.h"

typedef struct {
    char starts[ASTRA_SERVICE_MANAGER_MAX_SERVICES];
    char stops[ASTRA_SERVICE_MANAGER_MAX_SERVICES];
    size_t start_count;
    size_t stop_count;
} AstraTestServiceTrace;

typedef struct {
    unsigned int starts;
    unsigned int stops;
    AstraServiceHealth health;
    bool fail_start;
    bool fail_stop;
    unsigned int fail_start_remaining;
    AstraStatus start_failure_status;
    char id;
    AstraTestServiceTrace* trace;
} AstraTestServiceState;

static AstraServiceDefinition astra_test_service_definition(
    const char* name,
    AstraTestServiceState* state);

static AstraResult astra_test_service_start(void* context) {
    AstraTestServiceState* state = context;
    state->starts++;
    if(state->trace) {
        state->trace->starts[state->trace->start_count++] = state->id;
    }
    if(state->fail_start || state->fail_start_remaining > 0) {
        if(state->fail_start_remaining > 0) {
            state->fail_start_remaining--;
        }
        AstraStatus status = state->start_failure_status == AstraStatusOk ?
                                 AstraStatusError :
                                 state->start_failure_status;
        return astra_result_error(status, "start failed");
    }
    return astra_result_ok();
}

static bool astra_test_service_recovery_policy(void) {
    AstraRuntimeContext rollback_context;
    AstraTestServiceTrace rollback_trace = {0};
    AstraTestServiceState rollback_states[4] = {
        {.health = AstraServiceHealthHealthy, .id = 'A', .trace = &rollback_trace},
        {.health = AstraServiceHealthHealthy, .id = 'B', .trace = &rollback_trace},
        {.health = AstraServiceHealthHealthy,
         .fail_start = true,
         .start_failure_status = AstraStatusTimeout,
         .id = 'C',
         .trace = &rollback_trace},
        {.health = AstraServiceHealthHealthy, .id = 'D', .trace = &rollback_trace},
    };
    const char* depends_a[] = {"alpha"};
    const char* depends_b[] = {"bravo"};
    const char* depends_c[] = {"charlie"};
    AstraServiceDefinition alpha = astra_test_service_definition("alpha", &rollback_states[0]);
    AstraServiceDefinition bravo = astra_test_service_definition("bravo", &rollback_states[1]);
    AstraServiceDefinition charlie = astra_test_service_definition("charlie", &rollback_states[2]);
    AstraServiceDefinition delta = astra_test_service_definition("delta", &rollback_states[3]);
    bravo.dependencies = depends_a;
    bravo.dependency_count = 1;
    charlie.dependencies = depends_b;
    charlie.dependency_count = 1;
    delta.dependencies = depends_c;
    delta.dependency_count = 1;

    if(astra_runtime_context_init(&rollback_context).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&rollback_context, &delta).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&rollback_context, &charlie).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&rollback_context, &bravo).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&rollback_context, &alpha).status != AstraStatusOk) return false;
    if(astra_service_manager_start_all_context(&rollback_context).status != AstraStatusTimeout) return false;
    if(rollback_trace.start_count != 3 || memcmp(rollback_trace.starts, "ABC", 3) != 0) return false;
    if(rollback_trace.stop_count != 2 || memcmp(rollback_trace.stops, "BA", 2) != 0) return false;
    if(rollback_states[3].starts != 0 || rollback_states[2].stops != 0) return false;
    if(astra_service_manager_find_context(&rollback_context, "alpha")->state != AstraServiceStateStopped) return false;
    if(astra_service_manager_validate_context(&rollback_context).status != AstraStatusOk) return false;

    AstraResult failure;
    if(astra_service_manager_last_start_failure_context(&rollback_context, &failure).status != AstraStatusOk) return false;
    if(failure.status != AstraStatusTimeout) return false;
    if(astra_service_manager_last_rollback_failure_context(&rollback_context, &failure).status != AstraStatusNotFound) return false;
    size_t starts_before = rollback_trace.start_count;
    if(astra_service_manager_recover_context(&rollback_context, AstraServiceRecoveryNoRetry).status != AstraStatusTimeout) return false;
    if(rollback_trace.start_count != starts_before) return false;

    AstraRuntimeContext rollback_failure_context;
    AstraTestServiceTrace rollback_failure_trace = {0};
    AstraTestServiceState rollback_failure_states[2] = {
        {.health = AstraServiceHealthHealthy,
         .fail_stop = true,
         .id = 'A',
         .trace = &rollback_failure_trace},
        {.health = AstraServiceHealthHealthy,
         .fail_start = true,
         .start_failure_status = AstraStatusTimeout,
         .id = 'B',
         .trace = &rollback_failure_trace},
    };
    alpha = astra_test_service_definition("alpha", &rollback_failure_states[0]);
    bravo = astra_test_service_definition("bravo", &rollback_failure_states[1]);
    bravo.dependencies = depends_a;
    bravo.dependency_count = 1;
    if(astra_runtime_context_init(&rollback_failure_context).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&rollback_failure_context, &bravo).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&rollback_failure_context, &alpha).status != AstraStatusOk) return false;
    if(astra_service_manager_start_all_context(&rollback_failure_context).status != AstraStatusTimeout) return false;
    if(astra_service_manager_last_start_failure_context(&rollback_failure_context, &failure).status != AstraStatusOk) return false;
    if(failure.status != AstraStatusTimeout) return false;
    if(astra_service_manager_last_rollback_failure_context(&rollback_failure_context, &failure).status != AstraStatusOk) return false;
    if(failure.status != AstraStatusError) return false;
    if(astra_service_manager_find_context(&rollback_failure_context, "alpha")->state != AstraServiceStateRunning) return false;
    if(astra_service_manager_validate_context(&rollback_failure_context).status != AstraStatusOk) return false;
    rollback_failure_states[0].fail_stop = false;
    if(astra_service_manager_shutdown_context(&rollback_failure_context).status != AstraStatusOk) return false;

    AstraRuntimeContext retry_context;
    AstraTestServiceTrace retry_trace = {0};
    AstraTestServiceState retry_states[3] = {
        {.health = AstraServiceHealthHealthy, .id = 'A', .trace = &retry_trace},
        {.health = AstraServiceHealthHealthy,
         .fail_start_remaining = 1,
         .start_failure_status = AstraStatusTimeout,
         .id = 'B',
         .trace = &retry_trace},
        {.health = AstraServiceHealthHealthy, .id = 'C', .trace = &retry_trace},
    };
    alpha = astra_test_service_definition("alpha", &retry_states[0]);
    bravo = astra_test_service_definition("bravo", &retry_states[1]);
    charlie = astra_test_service_definition("charlie", &retry_states[2]);
    bravo.dependencies = depends_a;
    bravo.dependency_count = 1;
    charlie.dependencies = depends_b;
    charlie.dependency_count = 1;
    if(astra_runtime_context_init(&retry_context).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&retry_context, &charlie).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&retry_context, &bravo).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&retry_context, &alpha).status != AstraStatusOk) return false;
    if(astra_service_manager_start_all_context(&retry_context).status != AstraStatusTimeout) return false;
    if(astra_service_manager_recover_context(&retry_context, AstraServiceRecoveryRetryOnce).status != AstraStatusOk) return false;
    if(retry_trace.start_count != 5 || memcmp(retry_trace.starts, "ABABC", 5) != 0) return false;
    starts_before = retry_trace.start_count;
    if(astra_service_manager_recover_context(&retry_context, AstraServiceRecoveryRetryOnce).status != AstraStatusBusy) return false;
    if(retry_trace.start_count != starts_before) return false;
    if(astra_service_manager_validate_context(&retry_context).status != AstraStatusOk) return false;
    if(astra_service_manager_shutdown_context(&retry_context).status != AstraStatusOk) return false;

    AstraRuntimeContext continue_context;
    AstraTestServiceTrace continue_trace = {0};
    AstraTestServiceState continue_states[4] = {
        {.health = AstraServiceHealthHealthy, .id = 'A', .trace = &continue_trace},
        {.health = AstraServiceHealthHealthy,
         .fail_start = true,
         .start_failure_status = AstraStatusTimeout,
         .id = 'B',
         .trace = &continue_trace},
        {.health = AstraServiceHealthHealthy, .id = 'C', .trace = &continue_trace},
        {.health = AstraServiceHealthHealthy, .id = 'D', .trace = &continue_trace},
    };
    alpha = astra_test_service_definition("alpha", &continue_states[0]);
    bravo = astra_test_service_definition("bravo", &continue_states[1]);
    charlie = astra_test_service_definition("charlie", &continue_states[2]);
    delta = astra_test_service_definition("delta", &continue_states[3]);
    bravo.dependencies = depends_a;
    bravo.dependency_count = 1;
    charlie.dependencies = depends_b;
    charlie.dependency_count = 1;
    if(astra_runtime_context_init(&continue_context).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&continue_context, &alpha).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&continue_context, &bravo).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&continue_context, &charlie).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&continue_context, &delta).status != AstraStatusOk) return false;
    if(astra_service_manager_start_all_context(&continue_context).status != AstraStatusTimeout) return false;
    if(astra_service_manager_recover_context(&continue_context, AstraServiceRecoveryContinueAllowed).status != AstraStatusTimeout) return false;
    if(continue_trace.start_count != 4 || memcmp(continue_trace.starts, "ABAD", 4) != 0) return false;
    if(astra_service_manager_find_context(&continue_context, "alpha")->state != AstraServiceStateRunning) return false;
    if(astra_service_manager_find_context(&continue_context, "bravo")->state != AstraServiceStateStopped) return false;
    if(astra_service_manager_find_context(&continue_context, "charlie")->state != AstraServiceStateStopped) return false;
    if(astra_service_manager_find_context(&continue_context, "delta")->state != AstraServiceStateRunning) return false;
    if(astra_service_manager_validate_context(&continue_context).status != AstraStatusOk) return false;
    if(astra_service_manager_shutdown_context(&continue_context).status != AstraStatusOk) return false;

    return true;
}

static AstraResult astra_test_service_stop(void* context) {
    AstraTestServiceState* state = context;
    state->stops++;
    if(state->trace) {
        state->trace->stops[state->trace->stop_count++] = state->id;
    }
    if(state->fail_stop) {
        return astra_result_error(AstraStatusError, "stop failed");
    }
    return astra_result_ok();
}

static bool astra_test_service_dependency_graph(void) {
    AstraRuntimeContext context;
    AstraRuntimeContext cycle_context;
    AstraRuntimeContext missing_context;
    AstraTestServiceTrace trace = {0};
    AstraTestServiceState states[3] = {
        {.health = AstraServiceHealthHealthy, .id = 'A', .trace = &trace},
        {.health = AstraServiceHealthHealthy, .id = 'B', .trace = &trace},
        {.health = AstraServiceHealthHealthy, .id = 'C', .trace = &trace},
    };
    const char* dependencies_b[] = {"alpha"};
    const char* dependencies_c[] = {"alpha", "bravo"};
    AstraServiceDefinition alpha = astra_test_service_definition("alpha", &states[0]);
    AstraServiceDefinition bravo = astra_test_service_definition("bravo", &states[1]);
    AstraServiceDefinition charlie = astra_test_service_definition("charlie", &states[2]);
    bravo.dependencies = dependencies_b;
    bravo.dependency_count = 1;
    charlie.dependencies = dependencies_c;
    charlie.dependency_count = 2;

    if(astra_runtime_context_init(&context).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&context, &charlie).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&context, &bravo).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&context, &alpha).status != AstraStatusOk) return false;
    if(astra_service_manager_validate_context(&context).status != AstraStatusOk) return false;

    AstraServiceHealth aggregate = AstraServiceHealthUnknown;
    if(astra_service_manager_health_aggregate_context(&context, &aggregate).status != AstraStatusOk) return false;
    if(aggregate != AstraServiceHealthStopped) return false;
    if(astra_service_manager_start_all_context(&context).status != AstraStatusOk) return false;
    if(trace.start_count != 3 || memcmp(trace.starts, "ABC", 3) != 0) return false;
    if(astra_service_manager_start_all_context(&context).status != AstraStatusOk) return false;
    if(trace.start_count != 3) return false;

    if(astra_service_manager_health_aggregate_context(&context, &aggregate).status != AstraStatusOk) return false;
    if(aggregate != AstraServiceHealthHealthy) return false;
    states[1].health = AstraServiceHealthDegraded;
    if(astra_service_manager_health_aggregate_context(&context, &aggregate).status != AstraStatusOk) return false;
    if(aggregate != AstraServiceHealthDegraded) return false;
    states[2].health = AstraServiceHealthUnhealthy;
    if(astra_service_manager_health_aggregate_context(&context, &aggregate).status != AstraStatusOk) return false;
    if(aggregate != AstraServiceHealthUnhealthy) return false;

    if(astra_service_manager_stop_context(&context, "alpha").status != AstraStatusBusy) return false;
    if(astra_service_manager_shutdown_context(&context).status != AstraStatusOk) return false;
    if(trace.stop_count != 3 || memcmp(trace.stops, "CBA", 3) != 0) return false;
    if(astra_service_manager_shutdown_context(&context).status != AstraStatusOk) return false;
    if(trace.stop_count != 3) return false;

    if(astra_runtime_context_init(&cycle_context).status != AstraStatusOk) return false;
    const char* dependencies_x[] = {"yankee"};
    const char* dependencies_y[] = {"xray"};
    AstraServiceDefinition xray = astra_test_service_definition("xray", &states[0]);
    AstraServiceDefinition yankee = astra_test_service_definition("yankee", &states[1]);
    xray.dependencies = dependencies_x;
    xray.dependency_count = 1;
    yankee.dependencies = dependencies_y;
    yankee.dependency_count = 1;
    if(astra_service_manager_register_context(&cycle_context, &xray).status != AstraStatusOk) return false;
    if(astra_service_manager_register_context(&cycle_context, &yankee).status != AstraStatusOk) return false;
    if(astra_service_manager_validate_context(&cycle_context).status != AstraStatusInvalidArgument) return false;
    if(astra_service_manager_start_all_context(&cycle_context).status != AstraStatusInvalidArgument) return false;

    if(astra_runtime_context_init(&missing_context).status != AstraStatusOk) return false;
    const char* dependencies_missing[] = {"absent"};
    AstraServiceDefinition missing = astra_test_service_definition("missing", &states[0]);
    missing.dependencies = dependencies_missing;
    missing.dependency_count = 1;
    if(astra_service_manager_register_context(&missing_context, &missing).status != AstraStatusOk) return false;
    if(astra_service_manager_validate_context(&missing_context).status != AstraStatusNotFound) return false;

    missing_context.service_manager.services[0].state = (AstraServiceState)99;
    if(astra_service_manager_validate_context(&missing_context).status != AstraStatusInternalError) return false;

    return true;
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

    return astra_test_service_dependency_graph() && astra_test_service_recovery_policy();
}
