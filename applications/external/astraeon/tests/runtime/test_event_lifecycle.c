#include "astra_tests.h"
#include "astra_event_builder.h"
#include "astra_event_lifecycle.h"

bool astra_test_event_lifecycle(void) {
    AstraEvent event;

    if(astra_event_lifecycle_init().status != AstraStatusOk) {
        return false;
    }

    if(astra_event_builder_init(&event, AstraEventTypeStatus).status != AstraStatusOk) {
        return false;
    }

    if(astra_event_lifecycle_mark_dispatching(&event).status != AstraStatusOk) {
        return false;
    }

    if(event.state != AstraEventStateDispatching) {
        return false;
    }

    if(astra_event_lifecycle_mark_completed(&event).status != AstraStatusOk) {
        return false;
    }

    if(event.state != AstraEventStateCompleted) {
        return false;
    }

    if(astra_event_lifecycle_mark_failed(&event).status != AstraStatusOk) {
        return false;
    }

    return event.state == AstraEventStateFailed &&
           astra_event_lifecycle_mark_failed(0).status == AstraStatusInvalidArgument;
}
