#include "astra_runtime_session.h"

AstraResult astra_runtime_session_init(AstraRuntimeSession* session) {
    if(!session) {
        return astra_result_error(AstraStatusInvalidArgument, "session is null");
    }

    session->session_id = 0;
    session->sequence = 0;
    session->state = AstraRuntimeSessionDisconnected;
    return astra_result_ok();
}

AstraResult astra_runtime_session_connect(AstraRuntimeSession* session, uint32_t session_id) {
    if(!session) {
        return astra_result_error(AstraStatusInvalidArgument, "session is null");
    }

    if(session_id == 0) {
        return astra_result_error(AstraStatusInvalidArgument, "session id is invalid");
    }

    session->session_id = session_id;
    session->sequence = 0;
    session->state = AstraRuntimeSessionConnected;
    return astra_result_ok();
}

AstraResult astra_runtime_session_disconnect(AstraRuntimeSession* session) {
    if(!session) {
        return astra_result_error(AstraStatusInvalidArgument, "session is null");
    }

    session->state = AstraRuntimeSessionClosed;
    return astra_result_ok();
}

uint32_t astra_runtime_session_next_sequence(AstraRuntimeSession* session) {
    if(!session) {
        return 0;
    }

    session->sequence++;
    return session->sequence;
}

AstraRuntimeSessionState astra_runtime_session_state(const AstraRuntimeSession* session) {
    if(!session) {
        return AstraRuntimeSessionDisconnected;
    }

    return session->state;
}
