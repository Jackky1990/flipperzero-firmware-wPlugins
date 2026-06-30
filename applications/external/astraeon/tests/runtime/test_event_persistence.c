#include "astra_tests.h"
#include "astra_event_builder.h"
#include "astra_event_persistence.h"

#include <string.h>

typedef struct {
    const char* path;
    char line[ASTRA_EVENT_PERSISTENCE_LINE_MAX];
    size_t size;
    unsigned int calls;
} AstraEventPersistenceTestSink;

static AstraResult astra_test_event_persistence_append(
    const char* path,
    const void* data,
    size_t size,
    void* context) {
    AstraEventPersistenceTestSink* sink = context;

    if(!sink || !data || size >= sizeof(sink->line)) {
        return astra_result_error(AstraStatusInvalidArgument, "event sink invalid");
    }

    sink->path = path;
    memcpy(sink->line, data, size);
    sink->line[size] = '\0';
    sink->size = size;
    sink->calls++;

    return astra_result_ok();
}

bool astra_test_event_persistence(void) {
    AstraStorage storage;
    AstraEventPersistence persistence;
    AstraEvent event;
    AstraEventPersistenceTestSink sink = {0};

    if(astra_event_persistence_init(0, &storage).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_event_persistence_init(&persistence, 0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_storage_init(&storage).status != AstraStatusOk) {
        return false;
    }

    if(astra_event_persistence_init(&persistence, &storage).status != AstraStatusOk) {
        return false;
    }

    if(astra_event_builder_init(&event, AstraEventTypeDiagnosticsReport).status != AstraStatusOk) {
        return false;
    }

    if(astra_event_persistence_append(0, &event).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_event_persistence_append(&persistence, 0).status != AstraStatusInvalidArgument) {
        return false;
    }

    strcpy(event.id, "EVT-FLP-0001");
    strcpy(event.source_device, "FLP-JACK-01");
    strcpy(event.target_device, "NODE01");
    event.category = AstraEventCategoryDiagnostics;
    event.state = AstraEventStateCompleted;
    event.priority = AstraEventPriorityNormal;
    event.flags = AstraEventFlagPersist;
    event.timestamp = 42;
    event.source_module = AstraModuleDiagnostics;
    event.payload_size = 7;
    event.status = AstraStatusOk;

    if(astra_event_persistence_append(&persistence, &event).status != AstraStatusNotFound) {
        return false;
    }

    if(astra_storage_set_append_callback(&storage, astra_test_event_persistence_append, &sink)
           .status != AstraStatusOk) {
        return false;
    }

    if(astra_event_persistence_append(&persistence, &event).status != AstraStatusOk) {
        return false;
    }

    return sink.calls == 1 &&
           strcmp(sink.path, ASTRA_STORAGE_EVENTS_PATH) == 0 &&
           strstr(sink.line, "\"type\":\"DIAGNOSTICS_REPORT\"") &&
           strstr(sink.line, "\"category\":\"DIAGNOSTICS\"") &&
           strstr(sink.line, "\"source_device\":\"FLP-JACK-01\"") &&
           sink.size == strlen(sink.line);
}
