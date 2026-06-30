#include "astra_tests.h"
#include "astra_policy.h"

bool astra_test_policy(void) {
    AstraPolicy policy;
    uint8_t payload[4] = {0};

    if(astra_policy_init(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_policy_validate_command(0, 1, payload, sizeof(payload)).status !=
       AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_policy_init(&policy).status != AstraStatusOk) {
        return false;
    }

    if(astra_policy_validate_command(&policy, 1, 0, 0).status != AstraStatusOk) {
        return false;
    }

    if(astra_policy_validate_command(&policy, 0, payload, sizeof(payload)).status !=
       AstraStatusPolicyDenied) {
        return false;
    }

    if(astra_policy_validate_command(
           &policy,
           ASTRA_POLICY_MAX_COMMAND_ID + 1,
           payload,
           sizeof(payload))
           .status != AstraStatusPolicyDenied) {
        return false;
    }

    if(astra_policy_validate_command(
           &policy,
           1,
           0,
           ASTRA_POLICY_MAX_PAYLOAD_SIZE + 1)
           .status != AstraStatusPolicyDenied) {
        return false;
    }

    if(astra_policy_validate_command(&policy, 1, 0, sizeof(payload)).status !=
       AstraStatusInvalidArgument) {
        return false;
    }

    policy.allow_empty_payload = false;

    if(astra_policy_validate_command(&policy, 1, 0, 0).status != AstraStatusPolicyDenied) {
        return false;
    }

    return astra_policy_validate_command(&policy, 1, payload, sizeof(payload)).status ==
           AstraStatusOk;
}
