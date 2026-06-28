#pragma once

#include <stddef.h>
#include <stdint.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    AstraResult (*send)(const uint8_t* data, size_t size);
    AstraResult (*receive)(uint8_t* buffer, size_t buffer_size, size_t* received_size);
} AstraRuntimeTransport;

AstraResult astra_runtime_transport_init(AstraRuntimeTransport* transport);

AstraResult astra_runtime_transport_send(
    AstraRuntimeTransport* transport,
    const uint8_t* data,
    size_t size);

AstraResult astra_runtime_transport_receive(
    AstraRuntimeTransport* transport,
    uint8_t* buffer,
    size_t buffer_size,
    size_t* received_size);

#ifdef __cplusplus
}
#endif
