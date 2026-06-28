#pragma once

#include <stdint.h>

#include "astra_result.h"

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

#ifdef __cplusplus
}
#endif
