#include "astra_tests.h"
#include "astra_event_bus.h"
#include "astra_event_builder.h"
#include "astra_registry.h"

static bool bus_handler_called = false;

static AstraResult astra_test_event_bus_handler(AstraEvent* event) {
    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    bus_handler_called = true;
    return astra_result_ok();
}

bool astra_test_event_bus(void) {
    AstraEvent event;
    bus_handler_called = false;

    if(astra_registry_init().status != AstraStatusOk) {
        return false;
    }

    if(astra_event_bus_init().status != AstraStatusOk) {
        return false;
    }

    if(astra_event_builder_init(&event, AstraEventTypeStatus).status != AstraStatusOk) {
        return false;
    }

    if(astra_event_bus_publish(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_event_bus_publish(&event).status != AstraStatusNotFound) {
        return false;
    }

    if(astra_registry_register(AstraEventTypeStatus, astra_test_event_bus_handler).status != AstraStatusOk) {
        return false;
    }

    if(astra_event_builder_init(&event, AstraEventTypeStatus).status != AstraStatusOk) {
        return false;
    }

    if(astra_event_bus_publish(&event).status != AstraStatusOk) {
        return false;
    }

    return bus_handler_called && event.state == AstraEventStateCompleted;
}
