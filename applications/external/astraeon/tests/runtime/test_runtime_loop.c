#include "astra_tests.h"
#include "astra_runtime_loop.h"
#include "astra_scheduler.h"
#include "astra_event_builder.h"
#include "astra_registry.h"

static bool runtime_loop_handler_called = false;

static AstraResult astra_test_runtime_loop_handler(AstraEvent* event) {
    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    runtime_loop_handler_called = true;
    return astra_result_ok();
}

bool astra_test_runtime_loop(void) {
    AstraEvent event;
    runtime_loop_handler_called = false;

    if(astra_runtime_loop_step().status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_registry_init().status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_loop_init().status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_loop_step().status != AstraStatusOk) {
        return false;
    }

    if(astra_registry_register(AstraEventTypeStatus, astra_test_runtime_loop_handler).status != AstraStatusOk) {
        return false;
    }

    if(astra_event_builder_init(&event, AstraEventTypeStatus).status != AstraStatusOk) {
        return false;
    }

    if(astra_scheduler_schedule(&event).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_loop_step().status != AstraStatusOk) {
        return false;
    }

    if(!runtime_loop_handler_called) {
        return false;
    }

    if(astra_runtime_loop_shutdown().status != AstraStatusOk) {
        return false;
    }

    return astra_runtime_loop_step().status == AstraStatusInvalidArgument;
}
