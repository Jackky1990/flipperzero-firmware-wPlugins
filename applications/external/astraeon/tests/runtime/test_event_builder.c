#include "astra_tests.h"
#include "astra_event_builder.h"

bool astra_test_event_builder(void) {
    AstraEvent event;
    AstraResult result = astra_event_builder_init(&event, AstraEventTypeStatus);

    return result.status == AstraStatusOk &&
           event.type == AstraEventTypeStatus &&
           event.state == AstraEventStateInitialized &&
           event.priority == AstraEventPriorityNormal;
}
