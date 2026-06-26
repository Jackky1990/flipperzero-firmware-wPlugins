#include "astra_event_builder.h"
#include <string.h>

static AstraResult astra_event_builder_validate_event(AstraEvent* event) {
    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    return astra_result_ok();
}

AstraResult astra_event_builder_reset(AstraEvent* event) {
    AstraResult validation = astra_event_builder_validate_event(event);
    if(validation.status != AstraStatusOk) {
        return validation;
    }

    memset(event, 0, sizeof(AstraEvent));
    event->state = AstraEventStateNew;
    event->priority = AstraEventPriorityNormal;
    event->status = AstraStatusOk;

    return astra_result_ok();
}

AstraResult astra_event_builder_init(AstraEvent* event, AstraEventType type) {
    AstraResult reset_result = astra_event_builder_reset(event);
    if(reset_result.status != AstraStatusOk) {
        return reset_result;
    }

    event->type = type;
    event->state = AstraEventStateInitialized;

    return astra_result_ok();
}
