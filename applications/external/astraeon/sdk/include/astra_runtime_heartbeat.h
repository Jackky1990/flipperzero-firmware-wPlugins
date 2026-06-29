#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t last_tx_tick;
    uint32_t last_rx_tick;
    uint32_t heartbeat_interval_ms;
    uint32_t timeout_ms;
} AstraRuntimeHeartbeat;

AstraResult astra_runtime_heartbeat_init(
    AstraRuntimeHeartbeat* heartbeat,
    uint32_t heartbeat_interval_ms,
    uint32_t timeout_ms);

AstraResult astra_runtime_heartbeat_mark_tx(
    AstraRuntimeHeartbeat* heartbeat,
    uint32_t now_ms);

AstraResult astra_runtime_heartbeat_mark_rx(
    AstraRuntimeHeartbeat* heartbeat,
    uint32_t now_ms);

bool astra_runtime_heartbeat_should_send(
    const AstraRuntimeHeartbeat* heartbeat,
    uint32_t now_ms);

bool astra_runtime_heartbeat_has_timed_out(
    const AstraRuntimeHeartbeat* heartbeat,
    uint32_t now_ms);

#ifdef __cplusplus
}
#endif
