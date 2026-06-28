#include "astra_tests.h"
#include "astra_runtime_transport.h"

static bool send_called = false;
static bool receive_called = false;

static AstraResult test_send(const uint8_t* data, size_t size) {
    if(!data || size == 0) {
        return astra_result_error(AstraStatusInvalidArgument, "data is invalid");
    }

    send_called = true;
    return astra_result_ok();
}

static AstraResult test_receive(uint8_t* buffer, size_t buffer_size, size_t* received_size) {
    if(!buffer || buffer_size == 0 || !received_size) {
        return astra_result_error(AstraStatusInvalidArgument, "receive output is invalid");
    }

    buffer[0] = 0x42;
    *received_size = 1;
    receive_called = true;
    return astra_result_ok();
}

bool astra_test_runtime_transport(void) {
    AstraRuntimeTransport transport;
    uint8_t data[4] = {1, 2, 3, 4};
    uint8_t buffer[4] = {0};
    size_t received_size = 0;

    send_called = false;
    receive_called = false;

    if(astra_runtime_transport_init(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_transport_init(&transport).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_transport_send(&transport, data, sizeof(data)).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_transport_receive(&transport, buffer, sizeof(buffer), &received_size).status != AstraStatusInvalidArgument) {
        return false;
    }

    transport.send = test_send;
    transport.receive = test_receive;

    if(astra_runtime_transport_send(&transport, 0, sizeof(data)).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_transport_send(&transport, data, 0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_transport_send(&transport, data, sizeof(data)).status != AstraStatusOk) {
        return false;
    }

    if(!send_called) {
        return false;
    }

    if(astra_runtime_transport_receive(&transport, 0, sizeof(buffer), &received_size).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_transport_receive(&transport, buffer, 0, &received_size).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_transport_receive(&transport, buffer, sizeof(buffer), 0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_transport_receive(&transport, buffer, sizeof(buffer), &received_size).status != AstraStatusOk) {
        return false;
    }

    return receive_called && received_size == 1 && buffer[0] == 0x42;
}
