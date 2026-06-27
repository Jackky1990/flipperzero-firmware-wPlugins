#include "astra_dispatcher.h"
#include "astra_registry.h"
#include "astra_handler.h"
#include "astra_event_lifecycle.h"

AstraResult astra_dispatcher_init(void) {
    return astra_result_ok();
}

AstraResult astra_dispatcher_dispatch(AstraEvent* event) {
    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    AstraEventHandler handler = astra_registry_find(event->type);
    if(!handler) {
        astra_event_lifecycle_mark_failed(event);
        return astra_result_error(AstraStatusNotFound, "handler not found");
    }

    astra_event_lifecycle_mark_dispatching(event);

    AstraResult result = astra_handler_invoke(handler, event);
    if(result.status != AstraStatusOk) {
        astra_event_lifecycle_mark_failed(event);
        return result;
    }

    astra_event_lifecycle_mark_completed(event);
    return astra_result_ok();
}

AstraResult astra_dispatcher_init_context(AstraRuntimeContext* context) {
    if(!context) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }

    return astra_dispatcher_init();
}

AstraResult astra_dispatcher_dispatch_context(AstraRuntimeContext* context, AstraEvent* event) {
    if(!context) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }

    return astra_dispatcher_dispatch(event);
}
