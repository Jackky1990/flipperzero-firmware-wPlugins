#pragma once

#include "astra_storage.h"

#include <storage/storage.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    Storage* storage;
    uint32_t append_calls;
    size_t append_bytes;
} AstraeonStorageAdapter;

AstraResult astraeon_storage_adapter_init(AstraeonStorageAdapter* adapter, Storage* storage);

AstraResult astraeon_storage_adapter_bind(
    AstraStorage* astra_storage,
    AstraeonStorageAdapter* adapter);

AstraResult astraeon_storage_adapter_append(
    const char* path,
    const void* data,
    size_t size,
    void* context);

#ifdef __cplusplus
}
#endif
