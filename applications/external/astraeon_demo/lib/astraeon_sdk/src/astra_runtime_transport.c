#include "astra_runtime_transport.h"

AstraResult astra_runtime_transport_init(AstraRuntimeTransport* transport) {
    if(!transport) {
        return astra_result_error(AstraStatusInvalidArgument, "transport is null");
    }

    transport->send = 0;
    transport->receive = 0;
    transport->capabilities.supports_stream = false;
    transport->capabilities.supports_packets = false;
    transport->capabilities.reliable = false;
    transport->capabilities.max_payload_size = 0;

    return astra_result_ok();
}

AstraResult astra_runtime_transport_send(
    AstraRuntimeTransport* transport,
    const uint8_t* data,
    size_t size) {
    if(!transport || !transport->send) {
        return astra_result_error(AstraStatusInvalidArgument, "transport send is null");
    }

    if(!data || size == 0) {
        return astra_result_error(AstraStatusInvalidArgument, "data is invalid");
    }

    if(transport->capabilities.max_payload_size > 0 &&
       size > transport->capabilities.max_payload_size) {
        return astra_result_error(AstraStatusBusy, "payload too large");
    }

    return transport->send(data, size);
}

AstraResult astra_runtime_transport_receive(
    AstraRuntimeTransport* transport,
    uint8_t* buffer,
    size_t buffer_size,
    size_t* received_size) {
    if(!transport || !transport->receive) {
        return astra_result_error(AstraStatusInvalidArgument, "transport receive is null");
    }

    if(!buffer || buffer_size == 0 || !received_size) {
        return astra_result_error(AstraStatusInvalidArgument, "receive output is invalid");
    }

    return transport->receive(buffer, buffer_size, received_size);
}

const AstraRuntimeTransportCapabilities*
astra_runtime_transport_capabilities(
    const AstraRuntimeTransport* transport) {
    if(!transport) {
        return 0;
    }

    return &transport->capabilities;
}
