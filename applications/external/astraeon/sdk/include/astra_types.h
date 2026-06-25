#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ASTRA_ID_MAX_LEN 64
#define ASTRA_NAME_MAX_LEN 64
#define ASTRA_MESSAGE_MAX_LEN 128

typedef uint64_t AstraTimestamp;
typedef char AstraEventId[ASTRA_ID_MAX_LEN];
typedef char AstraDeviceId[ASTRA_ID_MAX_LEN];
typedef char AstraModuleId[ASTRA_NAME_MAX_LEN];
typedef char AstraTransportId[ASTRA_NAME_MAX_LEN];
typedef uint32_t AstraCapabilityMask;

typedef struct {
    uint16_t major;
    uint16_t minor;
    uint16_t patch;
} AstraVersion;

typedef enum {
    AstraModuleSystem = 0,
    AstraModuleTransport,
    AstraModuleStorage,
    AstraModuleLogger,
    AstraModuleEvent,
    AstraModulePolicy,
    AstraModuleMission,
    AstraModuleInventory,
    AstraModuleDiagnostics,
    AstraModuleAutomation,
    AstraModuleApplication,
    AstraModuleUnknown,
} AstraModuleType;

const char* astra_module_type_to_string(AstraModuleType type);

#ifdef __cplusplus
}
#endif
