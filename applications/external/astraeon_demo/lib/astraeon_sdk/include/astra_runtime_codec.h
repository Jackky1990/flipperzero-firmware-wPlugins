#pragma once

#include <stddef.h>
#include <stdint.h>

#include "astra_result.h"
#include "astra_runtime_message.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASTRA_RUNTIME_CODEC_MAGIC 0x4153u
#define ASTRA_RUNTIME_CODEC_VERSION 1u
#define ASTRA_RUNTIME_CODEC_HEADER_SIZE 16u

AstraResult astra_runtime_message_encode(
    const AstraRuntimeMessage* message,
    uint8_t* buffer,
    size_t buffer_size,
    size_t* encoded_size);

AstraResult astra_runtime_message_decode(
    AstraRuntimeMessage* message,
    const uint8_t* buffer,
    size_t buffer_size);

#ifdef __cplusplus
}
#endif
