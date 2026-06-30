#include "astra_event_persistence.h"

#include "astra_status.h"
#include "astra_types.h"

#include <stdio.h>

AstraResult astra_event_persistence_init(
    AstraEventPersistence* persistence,
    AstraStorage* storage) {
    if(!persistence || !storage) {
        return astra_result_error(AstraStatusInvalidArgument, "event persistence init invalid");
    }

    persistence->storage = storage;

    return astra_result_ok();
}

AstraResult astra_event_persistence_append(
    AstraEventPersistence* persistence,
    const AstraEvent* event) {
    char line[ASTRA_EVENT_PERSISTENCE_LINE_MAX];

    if(!persistence || !persistence->storage || !event) {
        return astra_result_error(AstraStatusInvalidArgument, "event persistence append invalid");
    }

    int written = snprintf(
        line,
        sizeof(line),
        "{\"id\":\"%s\",\"type\":\"%s\",\"category\":\"%s\",\"state\":\"%s\","
        "\"priority\":\"%s\",\"flags\":%lu,\"timestamp\":%llu,"
        "\"source_module\":\"%s\",\"status\":\"%s\",\"source_device\":\"%s\","
        "\"target_device\":\"%s\",\"payload_size\":%lu}\n",
        event->id,
        astra_event_type_to_string(event->type),
        astra_event_category_to_string(event->category),
        astra_event_state_to_string(event->state),
        astra_event_priority_to_string(event->priority),
        (unsigned long)event->flags,
        (unsigned long long)event->timestamp,
        astra_module_type_to_string(event->source_module),
        astra_status_to_string(event->status),
        event->source_device,
        event->target_device,
        (unsigned long)event->payload_size);

    if(written < 0 || (size_t)written >= sizeof(line)) {
        return astra_result_error(AstraStatusInvalidArgument, "event persistence line too long");
    }

    return astra_storage_append(
        persistence->storage,
        ASTRA_STORAGE_EVENTS_PATH,
        line,
        (size_t)written);
}
