#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool ready;
    bool transport_ready;
    bool ping_ok;
    bool capabilities_ok;
    bool handshake_ok;
    bool heartbeat_ok;
    bool policy_ok;
    bool supports_stream;
    bool supports_packets;
    bool reliable;
    uint32_t diagnostic_runs;
    uint32_t max_payload_size;
    uint16_t protocol_version;
} AstraeonRuntimeContext;
