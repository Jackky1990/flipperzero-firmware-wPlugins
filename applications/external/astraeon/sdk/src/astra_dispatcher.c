#include "astra_dispatcher.h"
#include "astra_registry.h"
#include "astra_handler.h"

AstraResult astra_dispatcher_init(void) {
    return astra_result_ok();
}

AstraResult astra_dispatcher_dispatch(AstraEvent* event) {
    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    AstraEventHandler handler = astra_registry_find(event->type);
    if(!handler) {
        event->state = AstraEventStateFailed;
        return astra_result_error(AstraStatusNotFound, "handler not found");
    }

    event->state = AstraEventStateDispatching;

    AstraResult result = astra_handler_invoke(handler, event);
    if(result.status != AstraStatusOk) {
        event->state = AstraEventStateFailed;
        return result;
    }

    event->state = AstraEventStateCompleted;
    return astra_result_ok();
}
