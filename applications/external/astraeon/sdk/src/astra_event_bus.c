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

AstraResult astra_event_bus_init_context(AstraRuntimeContext* context) {
    if(!context) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }

    return astra_event_bus_init();
}

AstraResult astra_event_bus_publish_context(AstraRuntimeContext* context, AstraEvent* event) {
    if(!context) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }

    return astra_event_bus_publish(event);
}
