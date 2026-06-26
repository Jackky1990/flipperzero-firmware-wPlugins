#include "astra_tests.h"
#include "astra_event_builder.h"
#include "astra_event_queue.h"

bool astra_test_event_queue(void) {
    AstraEventQueue queue;
    AstraEvent event;
    AstraEvent out;

    if(astra_event_queue_init(&queue).status != AstraStatusOk) return false;
    if(astra_event_builder_init(&event, AstraEventTypeStatus).status != AstraStatusOk) return false;
    if(!astra_event_queue_is_empty(&queue)) return false;
    if(astra_event_queue_push(&queue, &event).status != AstraStatusOk) return false;
    if(astra_event_queue_size(&queue) != 1) return false;
    if(astra_event_queue_pop(&queue, &out).status != AstraStatusOk) return false;

    return out.type == AstraEventTypeStatus &&
           out.state == AstraEventStateQueued &&
           astra_event_queue_is_empty(&queue);
}
