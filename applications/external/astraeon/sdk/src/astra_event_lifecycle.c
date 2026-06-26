#include "astra_event_lifecycle.h"

static AstraResult astra_event_lifecycle_validate(AstraEvent* event) {
    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    return astra_result_ok();
}

AstraResult astra_event_lifecycle_init(void) {
    return astra_result_ok();
}

AstraResult astra_event_lifecycle_mark_dispatching(AstraEvent* event) {
    AstraResult validation = astra_event_lifecycle_validate(event);
    if(validation.status != AstraStatusOk) {
        return validation;
    }

    event->state = AstraEventStateDispatching;
    return astra_result_ok();
}

AstraResult astra_event_lifecycle_mark_completed(AstraEvent* event) {
    AstraResult validation = astra_event_lifecycle_validate(event);
    if(validation.status != AstraStatusOk) {
        return validation;
    }

    event->state = AstraEventStateCompleted;
    return astra_result_ok();
}

AstraResult astra_event_lifecycle_mark_failed(AstraEvent* event) {
    AstraResult validation = astra_event_lifecycle_validate(event);
    if(validation.status != AstraStatusOk) {
        return validation;
    }

    event->state = AstraEventStateFailed;
    return astra_result_ok();
}
