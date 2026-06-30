#include "astra_storage.h"

#include <string.h>

static int astra_storage_path_has_root(const char* path) {
    size_t root_length = strlen(ASTRA_STORAGE_ROOT);

    return strncmp(path, ASTRA_STORAGE_ROOT, root_length) == 0 &&
           (path[root_length] == '\0' || path[root_length] == '/');
}

AstraResult astra_storage_init(AstraStorage* storage) {
    if(!storage) {
        return astra_result_error(AstraStatusInvalidArgument, "storage is null");
    }

    storage->append = 0;
    storage->context = 0;

    return astra_result_ok();
}

AstraResult astra_storage_set_append_callback(
    AstraStorage* storage,
    AstraStorageAppendCallback append,
    void* context) {
    if(!storage || !append) {
        return astra_result_error(AstraStatusInvalidArgument, "storage callback invalid");
    }

    storage->append = append;
    storage->context = context;

    return astra_result_ok();
}

AstraResult astra_storage_validate_path(const char* path) {
    if(!path || path[0] == '\0') {
        return astra_result_error(AstraStatusInvalidArgument, "path is invalid");
    }

    if(!astra_storage_path_has_root(path)) {
        return astra_result_error(AstraStatusPermissionDenied, "path outside astraeon root");
    }

    return astra_result_ok();
}

AstraResult astra_storage_append(
    AstraStorage* storage,
    const char* path,
    const void* data,
    size_t size) {
    if(!storage) {
        return astra_result_error(AstraStatusInvalidArgument, "storage is null");
    }

    AstraResult path_result = astra_storage_validate_path(path);
    if(path_result.status != AstraStatusOk) {
        return path_result;
    }

    if(!data || size == 0) {
        return astra_result_error(AstraStatusInvalidArgument, "append data invalid");
    }

    if(!storage->append) {
        return astra_result_error(AstraStatusNotFound, "append callback missing");
    }

    return storage->append(path, data, size, storage->context);
}
