#include "astra_result.h"

AstraResult astra_result_ok(void) {
    AstraResult result = {
        .status = AstraStatusOk,
        .message = "OK",
        .code = 0,
        .context = 0,
    };
    return result;
}

AstraResult astra_result_error(AstraStatus status, const char* message) {
    AstraResult result = {
        .status = status,
        .message = message,
        .code = 0,
        .context = 0,
    };
    return result;
}

AstraResult astra_result_with_context(AstraStatus status, const char* message, uint32_t code, void* context) {
    AstraResult result = {
        .status = status,
        .message = message,
        .code = code,
        .context = context,
    };
    return result;
}
