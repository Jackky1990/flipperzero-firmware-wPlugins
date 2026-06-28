#include "astra_runtime_usb_transport.h"

static AstraResult astra_usb_transport_send(const uint8_t* data, size_t size) {
    if(!data || size == 0) {
        return astra_result_error(AstraStatusInvalidArgument, "usb send invalid");
    }

    return astra_result_error(AstraStatusNotFound, "usb transport not attached");
}

static AstraResult astra_usb_transport_receive(uint8_t* buffer, size_t buffer_size, size_t* received_size) {
    if(!buffer || buffer_size == 0 || !received_size) {
        return astra_result_error(AstraStatusInvalidArgument, "usb receive invalid");
    }

    *received_size = 0;
    return astra_result_error(AstraStatusNotFound, "usb transport not attached");
}

AstraResult astra_runtime_usb_transport_init(AstraRuntimeTransport* transport) {
    if(!transport) {
        return astra_result_error(AstraStatusInvalidArgument, "transport is null");
    }

    transport->send = astra_usb_transport_send;
    transport->receive = astra_usb_transport_receive;
    return astra_result_ok();
}
