#pragma once

#include <stddef.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASTRA_STORAGE_ROOT "/ext/astraeon"
#define ASTRA_STORAGE_EVENTS_PATH "/ext/astraeon/events/flipper_events.jsonl"
#define ASTRA_STORAGE_LOG_PATH "/ext/astraeon/logs/astra.log"
#define ASTRA_STORAGE_CONFIG_PATH "/ext/astraeon/config/device.json"
#define ASTRA_STORAGE_CACHE_PATH "/ext/astraeon/cache/"

typedef AstraResult (*AstraStorageAppendCallback)(
    const char* path,
    const void* data,
    size_t size,
    void* context);

typedef struct {
    AstraStorageAppendCallback append;
    void* context;
} AstraStorage;

AstraResult astra_storage_init(AstraStorage* storage);

AstraResult astra_storage_set_append_callback(
    AstraStorage* storage,
    AstraStorageAppendCallback append,
    void* context);

AstraResult astra_storage_validate_path(const char* path);

AstraResult astra_storage_append(
    AstraStorage* storage,
    const char* path,
    const void* data,
    size_t size);

#ifdef __cplusplus
}
#endif
