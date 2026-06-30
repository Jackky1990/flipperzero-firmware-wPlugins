#include "astra_policy.h"

AstraResult astra_policy_init(AstraPolicy* policy) {
    if(!policy) {
        return astra_result_error(AstraStatusInvalidArgument, "policy is null");
    }

    policy->max_command_id = ASTRA_POLICY_MAX_COMMAND_ID;
    policy->max_payload_size = ASTRA_POLICY_MAX_PAYLOAD_SIZE;
    policy->allow_empty_payload = true;

    return astra_result_ok();
}

AstraResult astra_policy_validate_command(
    const AstraPolicy* policy,
    uint32_t command_id,
    const void* payload,
    size_t payload_size) {
    if(!policy) {
        return astra_result_error(AstraStatusInvalidArgument, "policy is null");
    }

    if(command_id == 0 || command_id > policy->max_command_id) {
        return astra_result_error(AstraStatusPolicyDenied, "command id denied");
    }

    if(payload_size > policy->max_payload_size) {
        return astra_result_error(AstraStatusPolicyDenied, "payload too large");
    }

    if(!policy->allow_empty_payload && payload_size == 0) {
        return astra_result_error(AstraStatusPolicyDenied, "empty payload denied");
    }

    if(payload_size > 0 && !payload) {
        return astra_result_error(AstraStatusInvalidArgument, "payload is null");
    }

    return astra_result_ok();
}
