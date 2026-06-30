#include "astra_node01.h"

#include "astra_status.h"
#include "astra_types.h"

#include <stdio.h>

AstraResult astra_node01_init(AstraNode01* node, AstraRuntimeTransport* transport) {
    if(!node || !transport) {
        return astra_result_error(AstraStatusInvalidArgument, "node01 init invalid");
    }

    node->transport = transport;
    node->node_id = ASTRA_NODE01_ID;

    return astra_result_ok();
}

AstraResult astra_node01_send_event(AstraNode01* node, const AstraEvent* event) {
    char packet[ASTRA_NODE01_PACKET_MAX];

    if(!node || !node->transport || !event) {
        return astra_result_error(AstraStatusInvalidArgument, "node01 send invalid");
    }

    int written = snprintf(
        packet,
        sizeof(packet),
        "{\"node\":\"%s\",\"kind\":\"event\",\"id\":\"%s\",\"type\":\"%s\","
        "\"source_module\":\"%s\",\"status\":\"%s\",\"timestamp\":%llu}\n",
        node->node_id ? node->node_id : ASTRA_NODE01_ID,
        event->id,
        astra_event_type_to_string(event->type),
        astra_module_type_to_string(event->source_module),
        astra_status_to_string(event->status),
        (unsigned long long)event->timestamp);

    if(written < 0 || (size_t)written >= sizeof(packet)) {
        return astra_result_error(AstraStatusInvalidArgument, "node01 packet too long");
    }

    return astra_runtime_transport_send(node->transport, (const uint8_t*)packet, (size_t)written);
}
