#pragma once

#include "astra_event.h"
#include "astra_node01.h"
#include "astra_runtime_transport.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    AstraRuntimeTransport transport;
    AstraNode01 node;
} AstraNode01UsbBridge;

AstraResult astra_node01_usb_bridge_init(AstraNode01UsbBridge* bridge);

AstraResult astra_node01_usb_bridge_send_event(
    AstraNode01UsbBridge* bridge,
    const AstraEvent* event);

#ifdef __cplusplus
}
#endif
