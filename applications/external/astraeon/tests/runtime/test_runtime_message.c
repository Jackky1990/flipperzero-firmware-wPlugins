#include "astra_tests.h"
#include "astra_runtime_message.h"

bool astra_test_runtime_message(void) {
    AstraRuntimeMessage message;
    const char payload[] = "ping";

    if(astra_runtime_message_init(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_message_validate(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_message_init(&message).status != AstraStatusOk) {
        return false;
    }

    if(message.type != AstraRuntimeMessageInvalid) {
        return false;
    }

    if(astra_runtime_message_validate(&message).status != AstraStatusInvalidArgument) {
        return false;
    }

    message.type = AstraRuntimeMessageHeartbeat;
    message.source = 0;
    message.destination = 1;
    message.payload_size = 0;
    message.payload = 0;

    if(astra_runtime_message_validate(&message).status != AstraStatusOk) {
        return false;
    }

    message.type = AstraRuntimeMessageCommand;
    message.payload_size = sizeof(payload);
    message.payload = 0;

    if(astra_runtime_message_validate(&message).status != AstraStatusInvalidArgument) {
        return false;
    }

    message.payload = payload;

    return astra_runtime_message_validate(&message).status == AstraStatusOk;
}
