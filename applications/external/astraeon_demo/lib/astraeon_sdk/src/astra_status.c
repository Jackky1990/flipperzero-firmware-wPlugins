#include "astra_status.h"

const char* astra_status_to_string(AstraStatus status) {
    switch(status) {
    case AstraStatusOk:
        return "OK";
    case AstraStatusError:
        return "ERROR";
    case AstraStatusInvalidArgument:
        return "INVALID_ARGUMENT";
    case AstraStatusTimeout:
        return "TIMEOUT";
    case AstraStatusStorageError:
        return "STORAGE_ERROR";
    case AstraStatusProtocolError:
        return "PROTOCOL_ERROR";
    case AstraStatusPolicyDenied:
        return "POLICY_DENIED";
    case AstraStatusPermissionDenied:
        return "PERMISSION_DENIED";
    case AstraStatusNotFound:
        return "NOT_FOUND";
    case AstraStatusBusy:
        return "BUSY";
    case AstraStatusInternalError:
        return "INTERNAL_ERROR";
    default:
        return "UNKNOWN";
    }
}
