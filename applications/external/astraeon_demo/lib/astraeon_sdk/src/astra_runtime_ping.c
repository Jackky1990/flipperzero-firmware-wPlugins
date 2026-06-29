#include "astra_runtime_ping.h"
#include "astra_runtime_codec.h"

#include <string.h>

static const uint8_t ping_payload[] = {'P','I','N','G'};

AstraResult astra_runtime_ping_send(AstraRuntimeTransport* transport) {
    if(!transport) return astra_result_error(AstraStatusInvalidArgument, "transport is null");

    AstraRuntimeMessage msg;
    uint8_t buffer[64];
    size_t size = 0;

    astra_runtime_message_init(&msg);
    msg.type = AstraRuntimeMessageCommand;
    msg.source = 0;
    msg.destination = 1;
    msg.payload_size = sizeof(ping_payload);
    msg.payload = ping_payload;

    AstraResult r = astra_runtime_message_encode(&msg, buffer, sizeof(buffer), &size);
    if(r.status != AstraStatusOk) return r;

    return astra_runtime_transport_send(transport, buffer, size);
}

AstraResult astra_runtime_ping_receive(AstraRuntimeTransport* transport) {
    if(!transport) return astra_result_error(AstraStatusInvalidArgument, "transport is null");

    uint8_t buffer[64];
    size_t size = 0;
    AstraRuntimeMessage msg;

    AstraResult r = astra_runtime_transport_receive(transport, buffer, sizeof(buffer), &size);
    if(r.status != AstraStatusOk) return r;

    r = astra_runtime_message_decode(&msg, buffer, size);
    if(r.status != AstraStatusOk) return r;

    if(msg.payload_size != sizeof(ping_payload) || !msg.payload) {
        return astra_result_error(AstraStatusInvalidArgument, "bad ping");
    }

    if(memcmp(msg.payload, ping_payload, sizeof(ping_payload)) != 0) {
        return astra_result_error(AstraStatusInvalidArgument, "bad ping payload");
    }

    return astra_result_ok();
}
