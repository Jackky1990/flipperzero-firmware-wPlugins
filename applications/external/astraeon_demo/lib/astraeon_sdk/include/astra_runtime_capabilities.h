#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASTRA_RUNTIME_FEATURE_NONE 0u

typedef struct {
    uint16_t protocol_version;
    bool supports_stream;
    bool supports_packets;
    bool reliable;
    uint32_t max_payload_size;
    uint32_t feature_flags;
} AstraRuntimeCapabilities;

AstraResult astra_runtime_capabilities_init(AstraRuntimeCapabilities* capabilities);

AstraResult astra_runtime_capabilities_validate(const AstraRuntimeCapabilities* capabilities);

bool astra_runtime_capabilities_supports_stream(const AstraRuntimeCapabilities* capabilities);

bool astra_runtime_capabilities_supports_packets(const AstraRuntimeCapabilities* capabilities);

#ifdef __cplusplus
}
#endif
