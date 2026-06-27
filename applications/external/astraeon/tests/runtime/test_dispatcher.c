#include "astra_tests.h"
#include "astra_dispatcher.h"
#include "astra_event_builder.h"
#include "astra_registry.h"
#include "astra_runtime_context.h"

static bool handler_called = false;

static AstraResult astra_test_dispatcher_handler(AstraEvent* event) {
    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    handler_called = true;
    return astra_result_ok();
}

bool astra_test_dispatcher(void) {
    AstraEvent event;
    AstraRuntimeContext context;

    handler_called = false;

    if(astra_registry_init().status != AstraStatusOk) {
        return false;
    }

    if(astra_dispatcher_init().status != AstraStatusOk) {
        return false;
    }

    if(astra_event_builder_init(&event, AstraEventTypeStatus).status != AstraStatusOk) {
        return false;
    }

    if(astra_dispatcher_dispatch(&event).status != AstraStatusNotFound) {
        return false;
    }

    if(event.state != AstraEventStateFailed) {
        return false;
    }

    if(astra_registry_register(AstraEventTypeStatus, astra_test_dispatcher_handler).status != AstraStatusOk) {
        return false;
    }

    if(astra_event_builder_init(&event, AstraEventTypeStatus).status != AstraStatusOk) {
        return false;
    }

    if(astra_dispatcher_dispatch(&event).status != AstraStatusOk) {
        return false;
    }

    if(!handler_called || event.state != AstraEventStateCompleted) {
        return false;
    }

    handler_called = false;

    if(astra_runtime_context_init(&context).status != AstraStatusOk) {
        return false;
    }

    if(astra_dispatcher_init_context(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_dispatcher_dispatch_context(0, &event).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_registry_init_context(&context).status != AstraStatusOk) {
        return false;
    }

    if(astra_dispatcher_init_context(&context).status != AstraStatusOk) {
        return false;
    }

    if(astra_event_builder_init(&event, AstraEventTypeStatus).status != AstraStatusOk) {
        return false;
    }

    if(astra_dispatcher_dispatch_context(&context, &event).status != AstraStatusNotFound) {
        return false;
    }

    if(astra_registry_register_context(&context, AstraEventTypeStatus, astra_test_dispatcher_handler).status != AstraStatusOk) {
        return false;
    }

    if(astra_event_builder_init(&event, AstraEventTypeStatus).status != AstraStatusOk) {
        return false;
    }

    if(astra_dispatcher_dispatch_context(&context, &event).status != AstraStatusOk) {
        return false;
    }

    return handler_called && event.state == AstraEventStateCompleted;
}
