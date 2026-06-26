#include "astra_handler.h"

AstraResult astra_handler_init(void) {
    return astra_result_ok();
}

AstraResult astra_handler_invoke(AstraEventHandler handler, AstraEvent* event) {
    if(!handler) {
        return astra_result_error(AstraStatusInvalidArgument, "handler is null");
    }

    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    return handler(event);
}
