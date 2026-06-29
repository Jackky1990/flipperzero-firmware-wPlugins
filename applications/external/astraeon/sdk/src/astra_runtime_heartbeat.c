#include "astra_runtime_heartbeat.h"

AstraResult astra_runtime_heartbeat_init(
    AstraRuntimeHeartbeat* heartbeat,
    uint32_t heartbeat_interval_ms,
    uint32_t timeout_ms) {
    if(!heartbeat) {
        return astra_result_error(AstraStatusInvalidArgument, "heartbeat is null");
    }

    if(heartbeat_interval_ms == 0 || timeout_ms == 0 || timeout_ms <= heartbeat_interval_ms) {
        return astra_result_error(AstraStatusInvalidArgument, "heartbeat timing is invalid");
    }

    heartbeat->last_tx_tick = 0;
    heartbeat->last_rx_tick = 0;
    heartbeat->heartbeat_interval_ms = heartbeat_interval_ms;
    heartbeat->timeout_ms = timeout_ms;

    return astra_result_ok();
}

AstraResult astra_runtime_heartbeat_mark_tx(
    AstraRuntimeHeartbeat* heartbeat,
    uint32_t now_ms) {
    if(!heartbeat) {
        return astra_result_error(AstraStatusInvalidArgument, "heartbeat is null");
    }

    heartbeat->last_tx_tick = now_ms;
    return astra_result_ok();
}

AstraResult astra_runtime_heartbeat_mark_rx(
    AstraRuntimeHeartbeat* heartbeat,
    uint32_t now_ms) {
    if(!heartbeat) {
        return astra_result_error(AstraStatusInvalidArgument, "heartbeat is null");
    }

    heartbeat->last_rx_tick = now_ms;
    return astra_result_ok();
}

bool astra_runtime_heartbeat_should_send(
    const AstraRuntimeHeartbeat* heartbeat,
    uint32_t now_ms) {
    if(!heartbeat) {
        return false;
    }

    return (now_ms - heartbeat->last_tx_tick) >= heartbeat->heartbeat_interval_ms;
}

bool astra_runtime_heartbeat_has_timed_out(
    const AstraRuntimeHeartbeat* heartbeat,
    uint32_t now_ms) {
    if(!heartbeat) {
        return true;
    }

    return (now_ms - heartbeat->last_rx_tick) >= heartbeat->timeout_ms;
}
