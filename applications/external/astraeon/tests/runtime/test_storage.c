#include "astra_tests.h"
#include "astra_storage.h"

#include <string.h>

typedef struct {
    const char* path;
    const void* data;
    size_t size;
    unsigned int calls;
} AstraStorageTestSink;

static AstraResult astra_test_storage_append(
    const char* path,
    const void* data,
    size_t size,
    void* context) {
    AstraStorageTestSink* sink = context;

    if(!sink) {
        return astra_result_error(AstraStatusInvalidArgument, "sink is null");
    }

    sink->path = path;
    sink->data = data;
    sink->size = size;
    sink->calls++;

    return astra_result_ok();
}

bool astra_test_storage(void) {
    AstraStorage storage;
    AstraStorageTestSink sink = {0};
    const char payload[] = "event\n";

    if(astra_storage_init(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_storage_validate_path(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_storage_validate_path("/tmp/astra.log").status != AstraStatusPermissionDenied) {
        return false;
    }

    if(astra_storage_validate_path("/ext/astraeonx/events.jsonl").status !=
       AstraStatusPermissionDenied) {
        return false;
    }

    if(astra_storage_validate_path(ASTRA_STORAGE_EVENTS_PATH).status != AstraStatusOk) {
        return false;
    }

    if(astra_storage_init(&storage).status != AstraStatusOk) {
        return false;
    }

    if(astra_storage_append(&storage, ASTRA_STORAGE_EVENTS_PATH, payload, sizeof(payload)).status !=
       AstraStatusNotFound) {
        return false;
    }

    if(astra_storage_set_append_callback(&storage, 0, &sink).status !=
       AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_storage_set_append_callback(&storage, astra_test_storage_append, &sink).status !=
       AstraStatusOk) {
        return false;
    }

    if(astra_storage_append(&storage, "/tmp/astra.log", payload, sizeof(payload)).status !=
       AstraStatusPermissionDenied) {
        return false;
    }

    if(astra_storage_append(&storage, ASTRA_STORAGE_EVENTS_PATH, 0, sizeof(payload)).status !=
       AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_storage_append(&storage, ASTRA_STORAGE_EVENTS_PATH, payload, 0).status !=
       AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_storage_append(&storage, ASTRA_STORAGE_EVENTS_PATH, payload, sizeof(payload)).status !=
       AstraStatusOk) {
        return false;
    }

    return sink.calls == 1 &&
           strcmp(sink.path, ASTRA_STORAGE_EVENTS_PATH) == 0 &&
           sink.data == payload &&
           sink.size == sizeof(payload);
}
