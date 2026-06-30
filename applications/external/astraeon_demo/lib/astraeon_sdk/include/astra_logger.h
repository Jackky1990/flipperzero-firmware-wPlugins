#pragma once

#include "astra_result.h"
#include "astra_storage.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASTRA_LOGGER_LINE_MAX 128

typedef enum {
    AstraLogLevelDebug = 0,
    AstraLogLevelInfo,
    AstraLogLevelWarn,
    AstraLogLevelError,
    AstraLogLevelCount,
} AstraLogLevel;

typedef struct {
    AstraStorage* storage;
} AstraLogger;

AstraResult astra_logger_init(AstraLogger* logger, AstraStorage* storage);

const char* astra_logger_level_name(AstraLogLevel level);

AstraResult astra_logger_log(AstraLogger* logger, AstraLogLevel level, const char* message);

#ifdef __cplusplus
}
#endif
