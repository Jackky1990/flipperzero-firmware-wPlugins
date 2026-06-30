#include "storage_adapter.h"

static bool astraeon_storage_adapter_mkdir(Storage* storage, const char* path) {
    FS_Error error = storage_common_mkdir(storage, path);
    return error == FSE_OK || error == FSE_EXIST;
}

static bool astraeon_storage_adapter_ensure_dirs(Storage* storage) {
    return astraeon_storage_adapter_mkdir(storage, ASTRA_STORAGE_ROOT) &&
           astraeon_storage_adapter_mkdir(storage, "/ext/astraeon/events") &&
           astraeon_storage_adapter_mkdir(storage, "/ext/astraeon/logs") &&
           astraeon_storage_adapter_mkdir(storage, "/ext/astraeon/config") &&
           astraeon_storage_adapter_mkdir(storage, "/ext/astraeon/cache");
}

AstraResult astraeon_storage_adapter_init(AstraeonStorageAdapter* adapter, Storage* storage) {
    if(!adapter || !storage) {
        return astra_result_error(AstraStatusInvalidArgument, "storage adapter init invalid");
    }

    adapter->storage = storage;
    adapter->append_calls = 0;
    adapter->append_bytes = 0;

    if(!astraeon_storage_adapter_ensure_dirs(storage)) {
        return astra_result_error(AstraStatusStorageError, "storage directories unavailable");
    }

    return astra_result_ok();
}

AstraResult astraeon_storage_adapter_bind(
    AstraStorage* astra_storage,
    AstraeonStorageAdapter* adapter) {
    if(!astra_storage || !adapter) {
        return astra_result_error(AstraStatusInvalidArgument, "storage adapter bind invalid");
    }

    return astra_storage_set_append_callback(
        astra_storage,
        astraeon_storage_adapter_append,
        adapter);
}

AstraResult astraeon_storage_adapter_append(
    const char* path,
    const void* data,
    size_t size,
    void* context) {
    AstraeonStorageAdapter* adapter = context;

    if(!adapter || !adapter->storage || !path || !data || size == 0) {
        return astra_result_error(AstraStatusInvalidArgument, "storage adapter append invalid");
    }

    AstraResult path_result = astra_storage_validate_path(path);
    if(path_result.status != AstraStatusOk) {
        return path_result;
    }

    if(!astraeon_storage_adapter_ensure_dirs(adapter->storage)) {
        return astra_result_error(AstraStatusStorageError, "storage directories unavailable");
    }

    File* file = storage_file_alloc(adapter->storage);
    if(!file) {
        return astra_result_error(AstraStatusStorageError, "storage file allocation failed");
    }

    bool ok = storage_file_open(file, path, FSAM_WRITE, FSOM_OPEN_APPEND);
    size_t written = 0;

    if(ok) {
        written = storage_file_write(file, data, size);
        ok = written == size && storage_file_sync(file);
    }

    storage_file_close(file);
    storage_file_free(file);

    if(!ok) {
        return astra_result_error(AstraStatusStorageError, "storage append failed");
    }

    adapter->append_calls++;
    adapter->append_bytes += written;

    return astra_result_ok();
}
