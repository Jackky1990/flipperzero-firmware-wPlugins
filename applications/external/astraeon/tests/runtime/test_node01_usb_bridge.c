#include "astra_tests.h"
#include "astra_event_builder.h"
#include "astra_node01_usb_bridge.h"

#include <string.h>

bool astra_test_node01_usb_bridge(void) {
    AstraNode01UsbBridge bridge;
    AstraEvent event;

    if(astra_node01_usb_bridge_init(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_node01_usb_bridge_init(&bridge).status != AstraStatusOk) {
        return false;
    }

    const AstraRuntimeTransportCapabilities* caps =
        astra_runtime_transport_capabilities(&bridge.transport);

    if(!caps || !caps->supports_stream || caps->supports_packets || !caps->reliable ||
       caps->max_payload_size != 64) {
        return false;
    }

    if(strcmp(bridge.node.node_id, ASTRA_NODE01_ID) != 0) {
        return false;
    }

    if(astra_event_builder_init(&event, AstraEventTypeDiagnosticsReport).status != AstraStatusOk) {
        return false;
    }

    if(astra_node01_usb_bridge_send_event(0, &event).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_node01_usb_bridge_send_event(&bridge, 0).status != AstraStatusInvalidArgument) {
        return false;
    }

    strcpy(event.id, "EVT-FLP-USB-1");
    event.source_module = AstraModuleDiagnostics;
    event.timestamp = 11;
    event.status = AstraStatusOk;

    return astra_node01_usb_bridge_send_event(&bridge, &event).status == AstraStatusNotFound;
}
