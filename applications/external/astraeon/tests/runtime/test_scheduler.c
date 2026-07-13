#include "astra_tests.h"
#include "astra_scheduler.h"
#include "astra_event_builder.h"
#include "astra_event_bus.h"
#include "astra_registry.h"
#include "astra_runtime_context.h"

static bool scheduler_handler_called = false;
static unsigned int scheduler_handler_a_calls = 0;
static unsigned int scheduler_handler_b_calls = 0;

static AstraResult astra_test_scheduler_handler(AstraEvent* event) {
    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    scheduler_handler_called = true;
    return astra_result_ok();
}

static AstraResult astra_test_scheduler_handler_a(AstraEvent* event) {
    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    scheduler_handler_a_calls++;
    return astra_result_ok();
}

static AstraResult astra_test_scheduler_handler_b(AstraEvent* event) {
    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    scheduler_handler_b_calls++;
    return astra_result_ok();
}

bool astra_test_scheduler(void) {
    AstraEvent event;
    AstraRuntimeContext context_a;
    AstraRuntimeContext context_b;
    scheduler_handler_called = false;
    scheduler_handler_a_calls = 0;
    scheduler_handler_b_calls = 0;

    if(astra_registry_init().status != AstraStatusOk) return false;
    if(astra_event_bus_init().status != AstraStatusOk) return false;
    if(astra_scheduler_init().status != AstraStatusOk) return false;

    if(astra_scheduler_step().status != AstraStatusNotFound) return false;

    if(astra_registry_register(AstraEventTypeStatus, astra_test_scheduler_handler).status != AstraStatusOk) {
        return false;
    }

    if(astra_event_builder_init(&event, AstraEventTypeStatus).status != AstraStatusOk) {
        return false;
    }

    if(astra_scheduler_schedule(&event).status != AstraStatusOk) {
        return false;
    }

    if(astra_scheduler_step().status != AstraStatusOk) {
        return false;
    }

    if(!scheduler_handler_called) {
        return false;
    }

    scheduler_handler_called = false;

    if(astra_runtime_context_init(&context_a).status != AstraStatusOk) return false;
    if(astra_runtime_context_init(&context_b).status != AstraStatusOk) return false;
    if(astra_scheduler_init_context(0).status != AstraStatusInvalidArgument) return false;
    if(astra_registry_init_context(&context_a).status != AstraStatusOk) return false;
    if(astra_registry_init_context(&context_b).status != AstraStatusOk) return false;
    if(astra_scheduler_init_context(&context_a).status != AstraStatusOk) return false;
    if(astra_scheduler_init_context(&context_b).status != AstraStatusOk) return false;

    if(astra_registry_register_context(
           &context_a,
           AstraEventTypeStatus,
           astra_test_scheduler_handler_a).status != AstraStatusOk) {
        return false;
    }

    if(astra_registry_register_context(
           &context_b,
           AstraEventTypeStatus,
           astra_test_scheduler_handler_b).status != AstraStatusOk) {
        return false;
    }

    if(astra_event_builder_init(&event, AstraEventTypeStatus).status != AstraStatusOk) {
        return false;
    }

    if(astra_scheduler_schedule_context(&context_a, &event).status != AstraStatusOk) {
        return false;
    }

    if(astra_scheduler_step_context(&context_b).status != AstraStatusNotFound) {
        return false;
    }

    if(scheduler_handler_a_calls != 0 || scheduler_handler_b_calls != 0) {
        return false;
    }

    if(astra_scheduler_step_context(&context_a).status != AstraStatusOk) {
        return false;
    }

    if(scheduler_handler_a_calls != 1 || scheduler_handler_b_calls != 0) {
        return false;
    }

    if(astra_event_builder_init(&event, AstraEventTypeStatus).status != AstraStatusOk) {
        return false;
    }

    if(astra_scheduler_schedule_context(&context_b, &event).status != AstraStatusOk) {
        return false;
    }

    if(astra_scheduler_step_context(&context_b).status != AstraStatusOk) {
        return false;
    }

    return scheduler_handler_a_calls == 1 && scheduler_handler_b_calls == 1;
}
