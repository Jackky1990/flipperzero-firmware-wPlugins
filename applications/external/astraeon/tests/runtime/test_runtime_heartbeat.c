#include "astra_tests.h"
#include "astra_runtime_heartbeat.h"

bool astra_test_runtime_heartbeat(void) {
    AstraRuntimeHeartbeat heartbeat;

    if(astra_runtime_heartbeat_init(0, 1000, 3000).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_heartbeat_init(&heartbeat, 0, 3000).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_heartbeat_init(&heartbeat, 1000, 1000).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_heartbeat_init(&heartbeat, 1000, 3000).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_heartbeat_should_send(0, 1000)) {
        return false;
    }

    if(!astra_runtime_heartbeat_has_timed_out(0, 1000)) {
        return false;
    }

    if(astra_runtime_heartbeat_should_send(&heartbeat, 999)) {
        return false;
    }

    if(!astra_runtime_heartbeat_should_send(&heartbeat, 1000)) {
        return false;
    }

    if(astra_runtime_heartbeat_mark_tx(&heartbeat, 1000).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_heartbeat_should_send(&heartbeat, 1999)) {
        return false;
    }

    if(!astra_runtime_heartbeat_should_send(&heartbeat, 2000)) {
        return false;
    }

    if(astra_runtime_heartbeat_has_timed_out(&heartbeat, 2999)) {
        return false;
    }

    if(!astra_runtime_heartbeat_has_timed_out(&heartbeat, 3000)) {
        return false;
    }

    if(astra_runtime_heartbeat_mark_rx(&heartbeat, 3000).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_heartbeat_has_timed_out(&heartbeat, 5999)) {
        return false;
    }

    return astra_runtime_heartbeat_has_timed_out(&heartbeat, 6000);
}
