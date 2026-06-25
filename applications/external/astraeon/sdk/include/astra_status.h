#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    AstraStatusOk = 0,
    AstraStatusError,
    AstraStatusInvalidArgument,
    AstraStatusTimeout,
    AstraStatusStorageError,
    AstraStatusProtocolError,
    AstraStatusPolicyDenied,
    AstraStatusPermissionDenied,
    AstraStatusNotFound,
    AstraStatusBusy,
    AstraStatusInternalError,
} AstraStatus;

const char* astra_status_to_string(AstraStatus status);

#ifdef __cplusplus
}
#endif
