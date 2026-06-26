#include "astra_scheduler.h"
#include "astra_event_queue.h"
#include "astra_event_bus.h"

static AstraEventQueue scheduler_queue;

AstraResult astra_scheduler_init(void) {
    return astra_event_queue_init(&scheduler_queue);
}

AstraResult astra_scheduler_schedule(const AstraEvent* event) {
    return astra_event_queue_push(&scheduler_queue, event);
}

AstraResult astra_scheduler_step(void) {
    AstraEvent event;

    AstraResult pop_result = astra_event_queue_pop(&scheduler_queue, &event);
    if(pop_result.status != AstraStatusOk) {
        return pop_result;
    }

    return astra_event_bus_publish(&event);
}
