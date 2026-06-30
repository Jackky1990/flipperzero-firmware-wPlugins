#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASTRA_POLICY_MAX_COMMAND_ID 1024u
#define ASTRA_POLICY_MAX_PAYLOAD_SIZE 256u

typedef struct {
    uint32_t max_command_id;
    size_t max_payload_size;
    bool allow_empty_payload;
} AstraPolicy;

AstraResult astra_policy_init(AstraPolicy* policy);

AstraResult astra_policy_validate_command(
    const AstraPolicy* policy,
    uint32_t command_id,
    const void* payload,
    size_t payload_size);

#ifdef __cplusplus
}
#endif
