#include "astra_event_bus.h"
#include "astra_dispatcher.h"

AstraResult astra_event_bus_init(void) {
    return astra_dispatcher_init();
}

AstraResult astra_event_bus_publish(AstraEvent* event) {
    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    return astra_dispatcher_dispatch(event);
}
