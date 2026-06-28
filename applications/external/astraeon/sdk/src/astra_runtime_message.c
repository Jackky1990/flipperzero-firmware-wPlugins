#include "astra_runtime_message.h"

#include <string.h>

AstraResult astra_runtime_message_init(AstraRuntimeMessage* message) {
    if(!message) {
        return astra_result_error(AstraStatusInvalidArgument, "message is null");
    }

    memset(message, 0, sizeof(*message));
    message->type = AstraRuntimeMessageInvalid;

    return astra_result_ok();
}

AstraResult astra_runtime_message_validate(const AstraRuntimeMessage* message) {
    if(!message) {
        return astra_result_error(AstraStatusInvalidArgument, "message is null");
    }

    if(message->type == AstraRuntimeMessageInvalid) {
        return astra_result_error(AstraStatusInvalidArgument, "message type is invalid");
    }

    if(message->payload_size > 0 && !message->payload) {
        return astra_result_error(AstraStatusInvalidArgument, "payload is null");
    }

    return astra_result_ok();
}
