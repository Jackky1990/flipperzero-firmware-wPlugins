#pragma once

#include <stddef.h>
#include "astra_event.h"
#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASTRA_EVENT_QUEUE_CAPACITY 16

typedef struct {
    AstraEvent items[ASTRA_EVENT_QUEUE_CAPACITY];
    size_t head;
    size_t tail;
    size_t count;
} AstraEventQueue;

AstraResult astra_event_queue_init(AstraEventQueue* queue);

AstraResult astra_event_queue_clear(AstraEventQueue* queue);

AstraResult astra_event_queue_push(AstraEventQueue* queue, const AstraEvent* event);

AstraResult astra_event_queue_pop(AstraEventQueue* queue, AstraEvent* out_event);

AstraResult astra_event_queue_peek(const AstraEventQueue* queue, AstraEvent* out_event);

size_t astra_event_queue_size(const AstraEventQueue* queue);

bool astra_event_queue_is_empty(const AstraEventQueue* queue);

bool astra_event_queue_is_full(const AstraEventQueue* queue);

#ifdef __cplusplus
}
#endif
