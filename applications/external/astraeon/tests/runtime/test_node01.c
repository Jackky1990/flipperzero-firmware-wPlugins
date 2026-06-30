#include "astra_tests.h"
#include "astra_event_builder.h"
#include "astra_node01.h"

#include <string.h>

static char node01_packet[ASTRA_NODE01_PACKET_MAX];
static size_t node01_packet_size;
static unsigned int node01_send_calls;

static AstraResult astra_test_node01_send(const uint8_t* data, size_t size) {
    if(!data || size == 0 || size >= sizeof(node01_packet)) {
        return astra_result_error(AstraStatusInvalidArgument, "node01 test send invalid");
    }

    memcpy(node01_packet, data, size);
    node01_packet[size] = '\0';
    node01_packet_size = size;
    node01_send_calls++;

    return astra_result_ok();
}

bool astra_test_node01(void) {
    AstraRuntimeTransport transport;
    AstraNode01 node;
    AstraEvent event;

    node01_packet[0] = '\0';
    node01_packet_size = 0;
    node01_send_calls = 0;

    if(astra_node01_init(0, &transport).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_node01_init(&node, 0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_transport_init(&transport).status != AstraStatusOk) {
        return false;
    }

    if(astra_node01_init(&node, &transport).status != AstraStatusOk) {
        return false;
    }

    if(astra_event_builder_init(&event, AstraEventTypeDiagnosticsReport).status != AstraStatusOk) {
        return false;
    }

    if(astra_node01_send_event(0, &event).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_node01_send_event(&node, 0).status != AstraStatusInvalidArgument) {
        return false;
    }

    strcpy(event.id, "EVT-FLP-DIAG-1");
    event.source_module = AstraModuleDiagnostics;
    event.timestamp = 7;
    event.status = AstraStatusOk;

    if(astra_node01_send_event(&node, &event).status != AstraStatusInvalidArgument) {
        return false;
    }

    transport.send = astra_test_node01_send;
    transport.capabilities.supports_packets = true;
    transport.capabilities.reliable = true;
    transport.capabilities.max_payload_size = sizeof(node01_packet);

    if(astra_node01_send_event(&node, &event).status != AstraStatusOk) {
        return false;
    }

    return node01_send_calls == 1 &&
           node01_packet_size == strlen(node01_packet) &&
           strstr(node01_packet, "\"node\":\"NODE01\"") &&
           strstr(node01_packet, "\"type\":\"DIAGNOSTICS_REPORT\"") &&
           strstr(node01_packet, "\"source_module\":\"DIAGNOSTICS\"");
}
