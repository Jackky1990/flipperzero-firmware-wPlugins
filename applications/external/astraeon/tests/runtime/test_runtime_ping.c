#include "astra_tests.h"
#include "astra_runtime_ping.h"
#include "astra_runtime_loopback.h"

bool astra_test_runtime_ping(void) {
    AstraRuntimeTransport transport;

    if(astra_runtime_ping_send(0).status != AstraStatusInvalidArgument) return false;
    if(astra_runtime_ping_receive(0).status != AstraStatusInvalidArgument) return false;

    if(astra_runtime_transport_init(&transport).status != AstraStatusOk) return false;
    if(astra_runtime_loopback_init(&transport).status != AstraStatusOk) return false;

    if(astra_runtime_ping_send(&transport).status != AstraStatusOk) return false;
    return astra_runtime_ping_receive(&transport).status == AstraStatusOk;
}
