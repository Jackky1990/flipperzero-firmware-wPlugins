#include "astra_tests.h"
#include "astra_event_source.h"
#include "astra_event_builder.h"
#include "astra_runtime_loop.h"
#include "astra_registry.h"

static bool event_source_handler_called = false;

static AstraResult astra_test_event_source_handler(AstraEvent* event) {
    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    event_source_handler_called = true;
    return astra_result_ok();
}

bool astra_test_event_source(void) {
    AstraEvent event;
    event_source_handler_called = false;

    if(astra_registry_init().status != AstraStatusOk) return false;
    if(astra_runtime_loop_init().status != AstraStatusOk) return false;
    if(astra_event_source_init().status != AstraStatusOk) return false;

    if(astra_event_source_emit(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_registry_register(AstraEventTypeStatus, astra_test_event_source_handler).status != AstraStatusOk) {
        return false;
    }

    if(astra_event_builder_init(&event, AstraEventTypeStatus).status != AstraStatusOk) {
        return false;
    }

    if(astra_event_source_emit(&event).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_loop_step().status != AstraStatusOk) {
        return false;
    }

    return event_source_handler_called;
}
