#include "astra_tests.h"
#include "astra_logger.h"

#include <string.h>

typedef struct {
    const char* path;
    char line[ASTRA_LOGGER_LINE_MAX];
    size_t size;
    unsigned int calls;
} AstraLoggerTestSink;

static AstraResult astra_test_logger_append(
    const char* path,
    const void* data,
    size_t size,
    void* context) {
    AstraLoggerTestSink* sink = context;

    if(!sink || !data || size >= sizeof(sink->line)) {
        return astra_result_error(AstraStatusInvalidArgument, "logger sink invalid");
    }

    sink->path = path;
    memcpy(sink->line, data, size);
    sink->line[size] = '\0';
    sink->size = size;
    sink->calls++;

    return astra_result_ok();
}

bool astra_test_logger(void) {
    AstraStorage storage;
    AstraLogger logger;
    AstraLoggerTestSink sink = {0};
    char long_message[ASTRA_LOGGER_LINE_MAX] = {0};

    memset(long_message, 'x', sizeof(long_message) - 1);

    if(astra_logger_init(0, &storage).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_logger_init(&logger, 0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_logger_level_name(AstraLogLevelInfo) == 0) {
        return false;
    }

    if(astra_logger_level_name(AstraLogLevelCount) != 0) {
        return false;
    }

    if(astra_storage_init(&storage).status != AstraStatusOk) {
        return false;
    }

    if(astra_logger_init(&logger, &storage).status != AstraStatusOk) {
        return false;
    }

    if(astra_logger_log(0, AstraLogLevelInfo, "boot").status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_logger_log(&logger, AstraLogLevelCount, "boot").status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_logger_log(&logger, AstraLogLevelInfo, 0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_logger_log(&logger, AstraLogLevelInfo, "").status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_logger_log(&logger, AstraLogLevelInfo, long_message).status !=
       AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_logger_log(&logger, AstraLogLevelInfo, "boot").status != AstraStatusNotFound) {
        return false;
    }

    if(astra_storage_set_append_callback(&storage, astra_test_logger_append, &sink).status !=
       AstraStatusOk) {
        return false;
    }

    if(astra_logger_log(&logger, AstraLogLevelInfo, "boot").status != AstraStatusOk) {
        return false;
    }

    return sink.calls == 1 &&
           strcmp(sink.path, ASTRA_STORAGE_LOG_PATH) == 0 &&
           strcmp(sink.line, "[INFO] boot\n") == 0 &&
           sink.size == strlen("[INFO] boot\n");
}
