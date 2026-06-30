#pragma once

#include <stddef.h>
#include <stdint.h>
#include "astra_event_types.h"
#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    AstraEventStateNew = 0,
    AstraEventStateInitialized,
    AstraEventStateQueued,
    AstraEventStateDispatching,
    AstraEventStateHandled,
    AstraEventStateCompleted,
    AstraEventStateFailed,
    AstraEventStateUnknown,
} AstraEventState;

typedef enum {
    AstraEventPriorityLow = 0,
    AstraEventPriorityNormal,
    AstraEventPriorityHigh,
    AstraEventPriorityCritical,
} AstraEventPriority;

typedef enum {
    AstraEventFlagNone = 0,
    AstraEventFlagLogOnly = 1 << 0,
    AstraEventFlagRequiresAck = 1 << 1,
    AstraEventFlagPersist = 1 << 2,
    AstraEventFlagBroadcast = 1 << 3,
    AstraEventFlagLocalOnly = 1 << 4,
} AstraEventFlag;

typedef struct {
    AstraEventId id;
    AstraEventType type;
    AstraEventCategory category;
    AstraEventState state;
    AstraEventPriority priority;
    uint32_t flags;
    AstraTimestamp timestamp;
    AstraModuleType source_module;
    AstraDeviceId source_device;
    AstraDeviceId target_device;
    void* payload;
    size_t payload_size;
    AstraStatus status;
} AstraEvent;

const char* astra_event_state_to_string(AstraEventState state);
const char* astra_event_priority_to_string(AstraEventPriority priority);

#ifdef __cplusplus
}
#endif
