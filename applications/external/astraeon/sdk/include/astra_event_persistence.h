#pragma once

#include "astra_event.h"
#include "astra_storage.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASTRA_EVENT_PERSISTENCE_LINE_MAX 384

typedef struct {
    AstraStorage* storage;
} AstraEventPersistence;

AstraResult astra_event_persistence_init(
    AstraEventPersistence* persistence,
    AstraStorage* storage);

AstraResult astra_event_persistence_append(
    AstraEventPersistence* persistence,
    const AstraEvent* event);

#ifdef __cplusplus
}
#endif
