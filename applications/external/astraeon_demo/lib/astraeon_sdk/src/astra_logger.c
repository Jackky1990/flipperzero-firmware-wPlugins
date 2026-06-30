#include "astra_logger.h"

#include <stdio.h>

AstraResult astra_logger_init(AstraLogger* logger, AstraStorage* storage) {
    if(!logger || !storage) {
        return astra_result_error(AstraStatusInvalidArgument, "logger init invalid");
    }

    logger->storage = storage;

    return astra_result_ok();
}

const char* astra_logger_level_name(AstraLogLevel level) {
    switch(level) {
    case AstraLogLevelDebug:
        return "DEBUG";
    case AstraLogLevelInfo:
        return "INFO";
    case AstraLogLevelWarn:
        return "WARN";
    case AstraLogLevelError:
        return "ERROR";
    default:
        return 0;
    }
}

AstraResult astra_logger_log(AstraLogger* logger, AstraLogLevel level, const char* message) {
    char line[ASTRA_LOGGER_LINE_MAX];
    const char* level_name = astra_logger_level_name(level);

    if(!logger || !logger->storage || !message || message[0] == '\0') {
        return astra_result_error(AstraStatusInvalidArgument, "log arguments invalid");
    }

    if(!level_name) {
        return astra_result_error(AstraStatusInvalidArgument, "log level invalid");
    }

    int written = snprintf(line, sizeof(line), "[%s] %s\n", level_name, message);
    if(written < 0 || (size_t)written >= sizeof(line)) {
        return astra_result_error(AstraStatusInvalidArgument, "log line too long");
    }

    return astra_storage_append(logger->storage, ASTRA_STORAGE_LOG_PATH, line, (size_t)written);
}
