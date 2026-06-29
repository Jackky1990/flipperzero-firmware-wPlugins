#pragma once

#include <stdint.h>

#include "astra_result.h"
#include "astra_runtime_capabilities.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASTRA_RUNTIME_PROTOCOL_VERSION 1u

typedef struct {
    uint16_t protocol_version;
    uint16_t capability_flags;
    uint32_t runtime_id;
    uint32_t max_payload_size;
} AstraRuntimeHandshake;

AstraResult astra_runtime_handshake_init(AstraRuntimeHandshake* handshake);

AstraResult astra_runtime_handshake_validate(const AstraRuntimeHandshake* handshake);

uint16_t astra_runtime_protocol_version(void);

AstraResult astra_runtime_handshake_negotiate(
    const AstraRuntimeCapabilities* local,
    const AstraRuntimeCapabilities* remote,
    AstraRuntimeCapabilities* agreed);

#ifdef __cplusplus
}
#endif
