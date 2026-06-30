#pragma once

#include "astra_event.h"
#include "astra_runtime_transport.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASTRA_NODE01_ID "NODE01"
#define ASTRA_NODE01_PACKET_MAX 256

typedef struct {
    AstraRuntimeTransport* transport;
    const char* node_id;
} AstraNode01;

AstraResult astra_node01_init(AstraNode01* node, AstraRuntimeTransport* transport);

AstraResult astra_node01_send_event(AstraNode01* node, const AstraEvent* event);

#ifdef __cplusplus
}
#endif
