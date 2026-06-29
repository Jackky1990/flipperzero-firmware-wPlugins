#pragma once

#include "astra_result.h"
#include "astra_runtime_transport.h"

#ifdef __cplusplus
extern "C" {
#endif

AstraResult astra_runtime_ping_send(AstraRuntimeTransport* transport);
AstraResult astra_runtime_ping_receive(AstraRuntimeTransport* transport);

#ifdef __cplusplus
}
#endif
