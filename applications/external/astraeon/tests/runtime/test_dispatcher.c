#include "astra_tests.h"
#include "astra_dispatcher.h"
#include "astra_event_builder.h"

bool astra_test_dispatcher(void) {
    AstraEvent event;

    if(astra_dispatcher_init().status != AstraStatusOk) {
        return false;
    }

    if(astra_event_builder_init(&event, AstraEventTypeStatus).status != AstraStatusOk) {
        return false;
    }

    return astra_dispatcher_dispatch(&event).status == AstraStatusOk;
}
