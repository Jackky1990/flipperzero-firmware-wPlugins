#include "astra_tests.h"
#include "astra_scheduler.h"
#include "astra_event_builder.h"
#include "astra_event_bus.h"
#include "astra_registry.h"

static bool scheduler_handler_called = false;

static AstraResult astra_test_scheduler_handler(AstraEvent* event) {
    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    scheduler_handler_called = true;
    return astra_result_ok();
}

bool astra_test_scheduler(void) {
    AstraEvent event;
    scheduler_handler_called = false;

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

    return scheduler_handler_called;
}
