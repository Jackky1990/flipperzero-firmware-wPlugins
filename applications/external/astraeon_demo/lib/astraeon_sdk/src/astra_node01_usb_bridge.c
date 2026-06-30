#include "astra_node01_usb_bridge.h"

#include "astra_runtime_usb_transport.h"

#include <stdio.h>

AstraResult astra_node01_usb_bridge_init(AstraNode01UsbBridge* bridge) {
    if(!bridge) {
        return astra_result_error(AstraStatusInvalidArgument, "node01 usb bridge is null");
    }

    AstraResult transport_result = astra_runtime_transport_init(&bridge->transport);
    if(transport_result.status != AstraStatusOk) {
        return transport_result;
    }

    transport_result = astra_runtime_usb_transport_init(&bridge->transport);
    if(transport_result.status != AstraStatusOk) {
        return transport_result;
    }

    return astra_node01_init(&bridge->node, &bridge->transport);
}

AstraResult astra_node01_usb_bridge_send_event(
    AstraNode01UsbBridge* bridge,
    const AstraEvent* event) {
    char frame[64];

    if(!bridge) {
        return astra_result_error(AstraStatusInvalidArgument, "node01 usb bridge is null");
    }

    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "node01 usb event is null");
    }

    int written = snprintf(
        frame,
        sizeof(frame),
        "N01 EVT %s %lu\n",
        event->id,
        (unsigned long)event->timestamp);

    if(written < 0 || (size_t)written >= sizeof(frame)) {
        return astra_result_error(AstraStatusInvalidArgument, "node01 usb frame too long");
    }

    return astra_runtime_transport_send(&bridge->transport, (const uint8_t*)frame, (size_t)written);
}
