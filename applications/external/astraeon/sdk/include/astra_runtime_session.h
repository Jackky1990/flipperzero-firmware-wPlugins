#pragma once

#include <stdint.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    AstraRuntimeSessionDisconnected = 0,
    AstraRuntimeSessionConnecting,
    AstraRuntimeSessionConnected,
    AstraRuntimeSessionClosed,
} AstraRuntimeSessionState;

typedef struct {
    uint32_t session_id;
    uint32_t sequence;
    AstraRuntimeSessionState state;
} AstraRuntimeSession;

AstraResult astra_runtime_session_init(AstraRuntimeSession* session);
AstraResult astra_runtime_session_connect(AstraRuntimeSession* session, uint32_t session_id);
AstraResult astra_runtime_session_disconnect(AstraRuntimeSession* session);
uint32_t astra_runtime_session_next_sequence(AstraRuntimeSession* session);
AstraRuntimeSessionState astra_runtime_session_state(const AstraRuntimeSession* session);

#ifdef __cplusplus
}
#endif
