#include "astra_event_queue.h"
#include <string.h>

static AstraResult astra_event_queue_validate(AstraEventQueue* queue) {
    if(!queue) {
        return astra_result_error(AstraStatusInvalidArgument, "queue is null");
    }

    return astra_result_ok();
}

AstraResult astra_event_queue_init(AstraEventQueue* queue) {
    AstraResult validation = astra_event_queue_validate(queue);
    if(validation.status != AstraStatusOk) {
        return validation;
    }

    memset(queue, 0, sizeof(AstraEventQueue));
    return astra_result_ok();
}

AstraResult astra_event_queue_clear(AstraEventQueue* queue) {
    return astra_event_queue_init(queue);
}

AstraResult astra_event_queue_push(AstraEventQueue* queue, const AstraEvent* event) {
    AstraResult validation = astra_event_queue_validate(queue);
    if(validation.status != AstraStatusOk) {
        return validation;
    }

    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    if(astra_event_queue_is_full(queue)) {
        return astra_result_error(AstraStatusBusy, "queue is full");
    }

    queue->items[queue->tail] = *event;
    queue->items[queue->tail].state = AstraEventStateQueued;
    queue->tail = (queue->tail + 1) % ASTRA_EVENT_QUEUE_CAPACITY;
    queue->count++;

    return astra_result_ok();
}

AstraResult astra_event_queue_pop(AstraEventQueue* queue, AstraEvent* out_event) {
    AstraResult validation = astra_event_queue_validate(queue);
    if(validation.status != AstraStatusOk) {
        return validation;
    }

    if(!out_event) {
        return astra_result_error(AstraStatusInvalidArgument, "out_event is null");
    }

    if(astra_event_queue_is_empty(queue)) {
        return astra_result_error(AstraStatusNotFound, "queue is empty");
    }

    *out_event = queue->items[queue->head];
    queue->head = (queue->head + 1) % ASTRA_EVENT_QUEUE_CAPACITY;
    queue->count--;

    return astra_result_ok();
}

AstraResult astra_event_queue_peek(const AstraEventQueue* queue, AstraEvent* out_event) {
    if(!queue) {
        return astra_result_error(AstraStatusInvalidArgument, "queue is null");
    }

    if(!out_event) {
        return astra_result_error(AstraStatusInvalidArgument, "out_event is null");
    }

    if(astra_event_queue_is_empty(queue)) {
        return astra_result_error(AstraStatusNotFound, "queue is empty");
    }

    *out_event = queue->items[queue->head];
    return astra_result_ok();
}

size_t astra_event_queue_size(const AstraEventQueue* queue) {
    if(!queue) {
        return 0;
    }

    return queue->count;
}

bool astra_event_queue_is_empty(const AstraEventQueue* queue) {
    return !queue || queue->count == 0;
}

bool astra_event_queue_is_full(const AstraEventQueue* queue) {
    return queue && queue->count >= ASTRA_EVENT_QUEUE_CAPACITY;
}
