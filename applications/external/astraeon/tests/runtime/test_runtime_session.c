#include "astra_tests.h"
#include "astra_runtime_session.h"

bool astra_test_runtime_session(void) {
    AstraRuntimeSession session;

    if(astra_runtime_session_init(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_session_connect(0, 1).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_session_disconnect(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_session_next_sequence(0) != 0) {
        return false;
    }

    if(astra_runtime_session_state(0) != AstraRuntimeSessionDisconnected) {
        return false;
    }

    if(astra_runtime_session_init(&session).status != AstraStatusOk) {
        return false;
    }

    if(session.session_id != 0 ||
       session.sequence != 0 ||
       session.state != AstraRuntimeSessionDisconnected) {
        return false;
    }

    if(astra_runtime_session_connect(&session, 0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_session_connect(&session, 42).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_session_state(&session) != AstraRuntimeSessionConnected) {
        return false;
    }

    if(session.session_id != 42) {
        return false;
    }

    if(astra_runtime_session_next_sequence(&session) != 1) {
        return false;
    }

    if(astra_runtime_session_next_sequence(&session) != 2) {
        return false;
    }

    if(astra_runtime_session_disconnect(&session).status != AstraStatusOk) {
        return false;
    }

    return astra_runtime_session_state(&session) == AstraRuntimeSessionClosed;
}
