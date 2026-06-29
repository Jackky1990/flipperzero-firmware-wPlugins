#pragma once

#include <stdint.h>
#include <stddef.h>

#include "astra_result.h"
#include "astra_runtime_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    AstraRuntimeMessageInvalid = 0,
    AstraRuntimeMessageEvent,
    AstraRuntimeMessageCommand,
    AstraRuntimeMessageHeartbeat,
    AstraRuntimeMessagePing,
    AstraRuntimeMessagePong,
    AstraRuntimeMessageCapability,
    AstraRuntimeMessageResponse,
} AstraRuntimeMessageType;

typedef struct {
    AstraRuntimeMessageType type;
    AstraRuntimeId source;
    AstraRuntimeId destination;
    uint16_t payload_size;
    const void* payload;
} AstraRuntimeMessage;

AstraResult astra_runtime_message_init(AstraRuntimeMessage* message);

AstraResult astra_runtime_message_validate(const AstraRuntimeMessage* message);

#ifdef __cplusplus
}
#endif
