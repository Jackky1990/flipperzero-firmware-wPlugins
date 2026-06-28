#include "astra_event_bus.h"
#include "astra_dispatcher.h"
#include "astra_runtime_default.h"

AstraResult astra_event_bus_init(void) {
    return astra_event_bus_init_context(astra_runtime_default_context());
}

AstraResult astra_event_bus_publish(AstraEvent* event) {
    return astra_event_bus_publish_context(astra_runtime_default_context(), event);
}

AstraResult astra_event_bus_init_context(AstraRuntimeContext* context) {
    if(!context) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }

    return astra_dispatcher_init_context(context);
}

AstraResult astra_event_bus_publish_context(AstraRuntimeContext* context, AstraEvent* event) {
    if(!context) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }

    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    return astra_dispatcher_dispatch_context(context, event);
}
