#include "astra_event_source.h"
#include "astra_scheduler.h"

AstraResult astra_event_source_init(void) {
    return astra_scheduler_init();
}

AstraResult astra_event_source_emit(const AstraEvent* event) {
    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    return astra_scheduler_schedule(event);
}
