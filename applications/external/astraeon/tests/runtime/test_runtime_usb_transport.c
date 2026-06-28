#include "astra_tests.h"
#include "astra_runtime_usb_transport.h"

bool astra_test_runtime_usb_transport(void) {
    AstraRuntimeTransport transport;
    uint8_t data[1] = {0x42};
    uint8_t buffer[8] = {0};
    size_t received = 123;

    if(astra_runtime_usb_transport_init(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_transport_init(&transport).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_usb_transport_init(&transport).status != AstraStatusOk) {
        return false;
    }

    const AstraRuntimeTransportCapabilities* caps =
        astra_runtime_transport_capabilities(&transport);

    if(!caps || !caps->supports_stream || caps->supports_packets ||
       !caps->reliable || caps->max_payload_size != 64) {
        return false;
    }

    if(astra_runtime_transport_send(&transport, data, sizeof(data)).status != AstraStatusNotFound) {
        return false;
    }

    if(astra_runtime_transport_receive(&transport, buffer, sizeof(buffer), &received).status != AstraStatusNotFound) {
        return false;
    }

    return received == 0;
}
