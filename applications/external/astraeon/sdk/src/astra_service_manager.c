#include "astra_service_manager.h"

#include <string.h>

#include "astra_runtime_context.h"
#include "astra_runtime_default.h"

#define ASTRA_SERVICE_INDEX_NONE ASTRA_SERVICE_MANAGER_MAX_SERVICES

static AstraServiceManager* astra_service_manager_from_context(AstraRuntimeContext* context) {
    if(!context) {
        return 0;
    }
    return &context->service_manager;
}

static AstraResult astra_service_manager_validate_lookup(
    AstraRuntimeContext* context,
    const char* name) {
    if(!context) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }
    if(!name || name[0] == '\0') {
        return astra_result_error(AstraStatusInvalidArgument, "service name is invalid");
    }
    return astra_result_ok();
}

static AstraService* astra_service_manager_find_mutable(
    AstraServiceManager* manager,
    const char* name) {
    if(!manager || !name || name[0] == '\0') {
        return 0;
    }

    for(size_t index = 0; index < ASTRA_SERVICE_MANAGER_MAX_SERVICES; index++) {
        AstraService* service = &manager->services[index];
        if(service->used && strcmp(service->name, name) == 0) {
            return service;
        }
    }
    return 0;
}

static size_t astra_service_manager_find_index(
    AstraServiceManager* manager,
    const char* name) {
    AstraService* service = astra_service_manager_find_mutable(manager, name);
    if(!service) {
        return ASTRA_SERVICE_INDEX_NONE;
    }
    return (size_t)(service - manager->services);
}

static AstraResult astra_service_manager_validate_state(AstraServiceManager* manager) {
    if(!manager || manager->started_count > ASTRA_SERVICE_MANAGER_MAX_SERVICES) {
        return astra_result_error(AstraStatusInternalError, "service manager state is invalid");
    }

    bool recorded[ASTRA_SERVICE_MANAGER_MAX_SERVICES] = {false};
    for(size_t position = 0; position < manager->started_count; position++) {
        size_t index = manager->start_order[position];
        if(index >= ASTRA_SERVICE_MANAGER_MAX_SERVICES || recorded[index] ||
           !manager->services[index].used ||
           manager->services[index].state != AstraServiceStateRunning) {
            return astra_result_error(AstraStatusInternalError, "service start order is invalid");
        }
        recorded[index] = true;
    }

    for(size_t index = 0; index < ASTRA_SERVICE_MANAGER_MAX_SERVICES; index++) {
        AstraService* service = &manager->services[index];
        if(!service->used) {
            continue;
        }
        if(service->state != AstraServiceStateStopped &&
           service->state != AstraServiceStateRunning) {
            return astra_result_error(AstraStatusInternalError, "service state is invalid");
        }
        if((service->state == AstraServiceStateRunning) != recorded[index]) {
            return astra_result_error(AstraStatusInternalError, "service state is not recorded");
        }
    }
    if(manager->has_start_failure &&
       (manager->failed_service_index >= ASTRA_SERVICE_MANAGER_MAX_SERVICES ||
        !manager->services[manager->failed_service_index].used)) {
        return astra_result_error(AstraStatusInternalError, "failed service record is invalid");
    }
    if(manager->has_rollback_failure && !manager->has_start_failure) {
        return astra_result_error(AstraStatusInternalError, "rollback failure record is invalid");
    }
    return astra_result_ok();
}

static AstraResult astra_service_manager_visit(
    AstraServiceManager* manager,
    size_t index,
    unsigned char* marks,
    size_t* order,
    size_t* order_count) {
    if(marks[index] == 1) {
        return astra_result_error(AstraStatusInvalidArgument, "service dependency cycle detected");
    }
    if(marks[index] == 2) {
        return astra_result_ok();
    }

    marks[index] = 1;
    AstraService* service = &manager->services[index];
    for(size_t dependency = 0; dependency < service->dependency_count; dependency++) {
        size_t dependency_index = astra_service_manager_find_index(
            manager,
            service->dependencies[dependency]);
        if(dependency_index == ASTRA_SERVICE_INDEX_NONE) {
            return astra_result_error(AstraStatusNotFound, "service dependency not found");
        }
        AstraResult result = astra_service_manager_visit(
            manager,
            dependency_index,
            marks,
            order,
            order_count);
        if(result.status != AstraStatusOk) {
            return result;
        }
    }

    marks[index] = 2;
    order[(*order_count)++] = index;
    return astra_result_ok();
}

static AstraResult astra_service_manager_build_order(
    AstraServiceManager* manager,
    size_t target,
    size_t* order,
    size_t* order_count) {
    unsigned char marks[ASTRA_SERVICE_MANAGER_MAX_SERVICES] = {0};
    *order_count = 0;

    if(target != ASTRA_SERVICE_INDEX_NONE) {
        return astra_service_manager_visit(manager, target, marks, order, order_count);
    }

    for(size_t index = 0; index < ASTRA_SERVICE_MANAGER_MAX_SERVICES; index++) {
        if(manager->services[index].used) {
            AstraResult result = astra_service_manager_visit(
                manager,
                index,
                marks,
                order,
                order_count);
            if(result.status != AstraStatusOk) {
                return result;
            }
        }
    }
    return astra_result_ok();
}

static void astra_service_manager_record_start(AstraServiceManager* manager, size_t index) {
    for(size_t position = 0; position < manager->started_count; position++) {
        if(manager->start_order[position] == index) {
            return;
        }
    }
    manager->start_order[manager->started_count++] = index;
}

static void astra_service_manager_remove_start(AstraServiceManager* manager, size_t index) {
    for(size_t position = 0; position < manager->started_count; position++) {
        if(manager->start_order[position] == index) {
            for(size_t next = position + 1; next < manager->started_count; next++) {
                manager->start_order[next - 1] = manager->start_order[next];
            }
            manager->started_count--;
            return;
        }
    }
}

static AstraResult astra_service_manager_rollback_to(
    AstraServiceManager* manager,
    size_t baseline) {
    while(manager->started_count > baseline) {
        size_t index = manager->start_order[manager->started_count - 1];
        AstraService* service = &manager->services[index];
        AstraResult result = service->stop(service->context);
        if(result.status != AstraStatusOk) {
            return result;
        }
        service->state = AstraServiceStateStopped;
        manager->started_count--;
    }
    return astra_result_ok();
}

static void astra_service_manager_record_rollback_failure(
    AstraServiceManager* manager,
    AstraResult failure) {
    if(!manager->has_rollback_failure) {
        manager->last_rollback_failure = failure;
        manager->has_rollback_failure = true;
    }
}

static void astra_service_manager_begin_start(AstraServiceManager* manager) {
    manager->has_start_failure = false;
    manager->has_rollback_failure = false;
    manager->recovery_attempted = false;
    manager->failed_service_index = ASTRA_SERVICE_INDEX_NONE;
    memset(&manager->last_start_failure, 0, sizeof(manager->last_start_failure));
    memset(&manager->last_rollback_failure, 0, sizeof(manager->last_rollback_failure));
}

static AstraResult astra_service_manager_start_order(
    AstraServiceManager* manager,
    const size_t* order,
    size_t order_count,
    bool preserve_original_failure) {
    size_t baseline = manager->started_count;
    for(size_t position = 0; position < order_count; position++) {
        size_t index = order[position];
        AstraService* service = &manager->services[index];
        if(service->state == AstraServiceStateRunning) {
            continue;
        }
        AstraResult result = service->start(service->context);
        if(result.status != AstraStatusOk) {
            if(!preserve_original_failure || !manager->has_start_failure) {
                manager->last_start_failure = result;
                manager->failed_service_index = index;
                manager->has_start_failure = true;
            }

            AstraResult rollback_result = astra_service_manager_rollback_to(manager, baseline);
            if(rollback_result.status != AstraStatusOk) {
                astra_service_manager_record_rollback_failure(manager, rollback_result);
            }
            AstraResult state_result = astra_service_manager_validate_state(manager);
            if(state_result.status != AstraStatusOk) {
                astra_service_manager_record_rollback_failure(manager, state_result);
            }
            return result;
        }
        service->state = AstraServiceStateRunning;
        astra_service_manager_record_start(manager, index);
    }
    return astra_result_ok();
}

static AstraResult astra_service_manager_validate_definition(
    const AstraServiceDefinition* definition) {
    if(!definition || !definition->name || definition->name[0] == '\0') {
        return astra_result_error(AstraStatusInvalidArgument, "service definition is invalid");
    }
    if(strlen(definition->name) > ASTRA_SERVICE_NAME_MAX_LENGTH) {
        return astra_result_error(AstraStatusInvalidArgument, "service name is too long");
    }
    if(!definition->start || !definition->stop) {
        return astra_result_error(AstraStatusInvalidArgument, "service lifecycle is incomplete");
    }
    if(definition->dependency_count > ASTRA_SERVICE_MAX_DEPENDENCIES ||
       (definition->dependency_count > 0 && !definition->dependencies)) {
        return astra_result_error(AstraStatusInvalidArgument, "service dependencies are invalid");
    }
    for(size_t index = 0; index < definition->dependency_count; index++) {
        const char* dependency = definition->dependencies[index];
        if(!dependency || dependency[0] == '\0' ||
           strlen(dependency) > ASTRA_SERVICE_NAME_MAX_LENGTH ||
           strcmp(dependency, definition->name) == 0) {
            return astra_result_error(AstraStatusInvalidArgument, "service dependency is invalid");
        }
        for(size_t previous = 0; previous < index; previous++) {
            if(strcmp(dependency, definition->dependencies[previous]) == 0) {
                return astra_result_error(AstraStatusInvalidArgument, "service dependency is duplicated");
            }
        }
    }
    return astra_result_ok();
}

AstraResult astra_service_manager_init(void) {
    return astra_service_manager_init_context(astra_runtime_default_context());
}

AstraResult astra_service_manager_register(const AstraServiceDefinition* definition) {
    return astra_service_manager_register_context(astra_runtime_default_context(), definition);
}

AstraResult astra_service_manager_start(const char* name) {
    return astra_service_manager_start_context(astra_runtime_default_context(), name);
}

AstraResult astra_service_manager_stop(const char* name) {
    return astra_service_manager_stop_context(astra_runtime_default_context(), name);
}

const AstraService* astra_service_manager_find(const char* name) {
    return astra_service_manager_find_context(astra_runtime_default_context(), name);
}

AstraResult astra_service_manager_health(const char* name, AstraServiceHealth* health) {
    return astra_service_manager_health_context(astra_runtime_default_context(), name, health);
}

AstraResult astra_service_manager_validate(void) {
    return astra_service_manager_validate_context(astra_runtime_default_context());
}

AstraResult astra_service_manager_start_all(void) {
    return astra_service_manager_start_all_context(astra_runtime_default_context());
}

AstraResult astra_service_manager_shutdown(void) {
    return astra_service_manager_shutdown_context(astra_runtime_default_context());
}

AstraResult astra_service_manager_health_aggregate(AstraServiceHealth* health) {
    return astra_service_manager_health_aggregate_context(astra_runtime_default_context(), health);
}

AstraResult astra_service_manager_recover(AstraServiceRecoveryPolicy policy) {
    return astra_service_manager_recover_context(astra_runtime_default_context(), policy);
}

AstraResult astra_service_manager_last_start_failure(AstraResult* failure) {
    return astra_service_manager_last_start_failure_context(
        astra_runtime_default_context(),
        failure);
}

AstraResult astra_service_manager_last_rollback_failure(AstraResult* failure) {
    return astra_service_manager_last_rollback_failure_context(
        astra_runtime_default_context(),
        failure);
}

AstraResult astra_service_manager_init_context(AstraRuntimeContext* context) {
    AstraServiceManager* manager = astra_service_manager_from_context(context);
    if(!manager) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }
    memset(manager, 0, sizeof(*manager));
    return astra_result_ok();
}

AstraResult astra_service_manager_register_context(
    AstraRuntimeContext* context,
    const AstraServiceDefinition* definition) {
    AstraServiceManager* manager = astra_service_manager_from_context(context);
    if(!manager) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }

    AstraResult state_result = astra_service_manager_validate_state(manager);
    if(state_result.status != AstraStatusOk) {
        return state_result;
    }

    AstraResult validation = astra_service_manager_validate_definition(definition);
    if(validation.status != AstraStatusOk) {
        return validation;
    }
    if(astra_service_manager_find_mutable(manager, definition->name)) {
        return astra_result_error(AstraStatusBusy, "service is already registered");
    }

    for(size_t index = 0; index < ASTRA_SERVICE_MANAGER_MAX_SERVICES; index++) {
        AstraService* service = &manager->services[index];
        if(!service->used) {
            size_t name_length = strlen(definition->name);
            memcpy(service->name, definition->name, name_length + 1);
            service->start = definition->start;
            service->stop = definition->stop;
            service->health = definition->health;
            service->context = definition->context;
            service->dependency_count = definition->dependency_count;
            for(size_t dependency = 0; dependency < definition->dependency_count; dependency++) {
                size_t dependency_length = strlen(definition->dependencies[dependency]);
                memcpy(
                    service->dependencies[dependency],
                    definition->dependencies[dependency],
                    dependency_length + 1);
            }
            service->state = AstraServiceStateStopped;
            service->used = true;
            return astra_result_ok();
        }
    }

    return astra_result_error(AstraStatusBusy, "service manager is full");
}

AstraResult astra_service_manager_start_context(AstraRuntimeContext* context, const char* name) {
    AstraResult validation = astra_service_manager_validate_lookup(context, name);
    if(validation.status != AstraStatusOk) {
        return validation;
    }
    AstraServiceManager* manager = astra_service_manager_from_context(context);
    AstraResult state_result = astra_service_manager_validate_state(manager);
    if(state_result.status != AstraStatusOk) {
        return state_result;
    }
    AstraService* service = astra_service_manager_find_mutable(
        manager,
        name);
    if(!service) {
        return astra_result_error(AstraStatusNotFound, "service not found");
    }
    if(service->state == AstraServiceStateRunning) {
        return astra_result_error(AstraStatusBusy, "service is already running");
    }

    size_t order[ASTRA_SERVICE_MANAGER_MAX_SERVICES];
    size_t order_count;
    AstraResult result = astra_service_manager_build_order(
        manager,
        astra_service_manager_find_index(manager, name),
        order,
        &order_count);
    if(result.status != AstraStatusOk) {
        return result;
    }
    astra_service_manager_begin_start(manager);
    return astra_service_manager_start_order(manager, order, order_count, false);
}

AstraResult astra_service_manager_stop_context(AstraRuntimeContext* context, const char* name) {
    AstraResult validation = astra_service_manager_validate_lookup(context, name);
    if(validation.status != AstraStatusOk) {
        return validation;
    }
    AstraServiceManager* manager = astra_service_manager_from_context(context);
    AstraResult state_result = astra_service_manager_validate_state(manager);
    if(state_result.status != AstraStatusOk) {
        return state_result;
    }
    AstraService* service = astra_service_manager_find_mutable(manager, name);
    if(!service) {
        return astra_result_error(AstraStatusNotFound, "service not found");
    }
    if(service->state == AstraServiceStateStopped) {
        return astra_result_error(AstraStatusBusy, "service is already stopped");
    }

    for(size_t index = 0; index < ASTRA_SERVICE_MANAGER_MAX_SERVICES; index++) {
        AstraService* dependent = &manager->services[index];
        if(!dependent->used || dependent->state != AstraServiceStateRunning) {
            continue;
        }
        for(size_t dependency = 0; dependency < dependent->dependency_count; dependency++) {
            if(strcmp(dependent->dependencies[dependency], name) == 0) {
                return astra_result_error(AstraStatusBusy, "service has running dependents");
            }
        }
    }

    AstraResult result = service->stop(service->context);
    if(result.status == AstraStatusOk) {
        service->state = AstraServiceStateStopped;
        astra_service_manager_remove_start(
            manager,
            astra_service_manager_find_index(manager, name));
    }
    return result;
}

const AstraService* astra_service_manager_find_context(
    AstraRuntimeContext* context,
    const char* name) {
    return astra_service_manager_find_mutable(
        astra_service_manager_from_context(context),
        name);
}

AstraResult astra_service_manager_health_context(
    AstraRuntimeContext* context,
    const char* name,
    AstraServiceHealth* health) {
    if(!health) {
        return astra_result_error(AstraStatusInvalidArgument, "health output is null");
    }

    AstraResult validation = astra_service_manager_validate_lookup(context, name);
    if(validation.status != AstraStatusOk) {
        return validation;
    }

    AstraServiceManager* manager = astra_service_manager_from_context(context);
    AstraResult state_result = astra_service_manager_validate_state(manager);
    if(state_result.status != AstraStatusOk) {
        return state_result;
    }
    AstraService* service = astra_service_manager_find_mutable(manager, name);
    if(!service) {
        return astra_result_error(AstraStatusNotFound, "service not found");
    }
    if(service->state == AstraServiceStateStopped) {
        *health = AstraServiceHealthStopped;
        return astra_result_ok();
    }
    if(!service->health) {
        *health = AstraServiceHealthUnknown;
        return astra_result_ok();
    }

    AstraServiceHealth current = service->health(service->context);
    if(current < AstraServiceHealthUnknown || current > AstraServiceHealthUnhealthy) {
        return astra_result_error(AstraStatusInternalError, "service health is invalid");
    }
    *health = current;
    return astra_result_ok();
}

AstraResult astra_service_manager_validate_context(AstraRuntimeContext* context) {
    AstraServiceManager* manager = astra_service_manager_from_context(context);
    if(!manager) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }
    AstraResult state_result = astra_service_manager_validate_state(manager);
    if(state_result.status != AstraStatusOk) {
        return state_result;
    }

    size_t order[ASTRA_SERVICE_MANAGER_MAX_SERVICES];
    size_t order_count;
    return astra_service_manager_build_order(
        manager,
        ASTRA_SERVICE_INDEX_NONE,
        order,
        &order_count);
}

AstraResult astra_service_manager_start_all_context(AstraRuntimeContext* context) {
    AstraServiceManager* manager = astra_service_manager_from_context(context);
    if(!manager) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }
    AstraResult validation = astra_service_manager_validate_context(context);
    if(validation.status != AstraStatusOk) {
        return validation;
    }

    size_t order[ASTRA_SERVICE_MANAGER_MAX_SERVICES];
    size_t order_count;
    AstraResult result = astra_service_manager_build_order(
        manager,
        ASTRA_SERVICE_INDEX_NONE,
        order,
        &order_count);
    if(result.status != AstraStatusOk) {
        return result;
    }
    astra_service_manager_begin_start(manager);
    return astra_service_manager_start_order(manager, order, order_count, false);
}

AstraResult astra_service_manager_shutdown_context(AstraRuntimeContext* context) {
    AstraServiceManager* manager = astra_service_manager_from_context(context);
    if(!manager) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }
    AstraResult validation = astra_service_manager_validate_state(manager);
    if(validation.status != AstraStatusOk) {
        return validation;
    }

    while(manager->started_count > 0) {
        size_t index = manager->start_order[manager->started_count - 1];
        AstraService* service = &manager->services[index];
        AstraResult result = service->stop(service->context);
        if(result.status != AstraStatusOk) {
            return result;
        }
        service->state = AstraServiceStateStopped;
        manager->started_count--;
    }
    return astra_result_ok();
}

static unsigned int astra_service_health_severity(AstraServiceHealth health) {
    switch(health) {
    case AstraServiceHealthHealthy:
        return 0;
    case AstraServiceHealthStopped:
        return 1;
    case AstraServiceHealthUnknown:
        return 2;
    case AstraServiceHealthDegraded:
        return 3;
    case AstraServiceHealthUnhealthy:
        return 4;
    default:
        return 5;
    }
}

AstraResult astra_service_manager_health_aggregate_context(
    AstraRuntimeContext* context,
    AstraServiceHealth* health) {
    AstraServiceManager* manager = astra_service_manager_from_context(context);
    if(!manager || !health) {
        return astra_result_error(AstraStatusInvalidArgument, "health arguments are invalid");
    }

    AstraResult validation = astra_service_manager_validate_state(manager);
    if(validation.status != AstraStatusOk) {
        return validation;
    }

    bool found = false;
    AstraServiceHealth aggregate = AstraServiceHealthHealthy;
    for(size_t index = 0; index < ASTRA_SERVICE_MANAGER_MAX_SERVICES; index++) {
        AstraService* service = &manager->services[index];
        if(!service->used) {
            continue;
        }
        AstraServiceHealth current;
        AstraResult result = astra_service_manager_health_context(context, service->name, &current);
        if(result.status != AstraStatusOk) {
            return result;
        }
        if(!found || astra_service_health_severity(current) >
                         astra_service_health_severity(aggregate)) {
            aggregate = current;
        }
        found = true;
    }

    *health = found ? aggregate : AstraServiceHealthUnknown;
    return astra_result_ok();
}

static bool astra_service_manager_dependencies_running(
    AstraServiceManager* manager,
    AstraService* service) {
    for(size_t dependency = 0; dependency < service->dependency_count; dependency++) {
        AstraService* required = astra_service_manager_find_mutable(
            manager,
            service->dependencies[dependency]);
        if(!required || required->state != AstraServiceStateRunning) {
            return false;
        }
    }
    return true;
}

AstraResult astra_service_manager_recover_context(
    AstraRuntimeContext* context,
    AstraServiceRecoveryPolicy policy) {
    AstraServiceManager* manager = astra_service_manager_from_context(context);
    if(!manager) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }
    if(policy < AstraServiceRecoveryNoRetry ||
       policy > AstraServiceRecoveryContinueAllowed) {
        return astra_result_error(AstraStatusInvalidArgument, "recovery policy is invalid");
    }
    if(!manager->has_start_failure) {
        return astra_result_error(AstraStatusNotFound, "startup failure not found");
    }
    if(policy == AstraServiceRecoveryNoRetry) {
        return manager->last_start_failure;
    }
    if(manager->recovery_attempted) {
        return astra_result_error(AstraStatusBusy, "recovery already attempted");
    }

    AstraResult validation = astra_service_manager_validate_context(context);
    if(validation.status != AstraStatusOk) {
        return validation;
    }
    manager->recovery_attempted = true;

    size_t order[ASTRA_SERVICE_MANAGER_MAX_SERVICES];
    size_t order_count;
    AstraResult order_result = astra_service_manager_build_order(
        manager,
        ASTRA_SERVICE_INDEX_NONE,
        order,
        &order_count);
    if(order_result.status != AstraStatusOk) {
        return order_result;
    }

    if(policy == AstraServiceRecoveryRetryOnce) {
        return astra_service_manager_start_order(manager, order, order_count, true);
    }

    for(size_t position = 0; position < order_count; position++) {
        size_t index = order[position];
        AstraService* service = &manager->services[index];
        if(service->state == AstraServiceStateRunning ||
           index == manager->failed_service_index ||
           !astra_service_manager_dependencies_running(manager, service)) {
            continue;
        }

        AstraResult result = service->start(service->context);
        if(result.status == AstraStatusOk) {
            service->state = AstraServiceStateRunning;
            astra_service_manager_record_start(manager, index);
        }
    }

    validation = astra_service_manager_validate_state(manager);
    if(validation.status != AstraStatusOk) {
        astra_service_manager_record_rollback_failure(manager, validation);
    }
    return manager->last_start_failure;
}

AstraResult astra_service_manager_last_start_failure_context(
    AstraRuntimeContext* context,
    AstraResult* failure) {
    AstraServiceManager* manager = astra_service_manager_from_context(context);
    if(!manager || !failure) {
        return astra_result_error(AstraStatusInvalidArgument, "failure arguments are invalid");
    }
    if(!manager->has_start_failure) {
        return astra_result_error(AstraStatusNotFound, "startup failure not found");
    }
    *failure = manager->last_start_failure;
    return astra_result_ok();
}

AstraResult astra_service_manager_last_rollback_failure_context(
    AstraRuntimeContext* context,
    AstraResult* failure) {
    AstraServiceManager* manager = astra_service_manager_from_context(context);
    if(!manager || !failure) {
        return astra_result_error(AstraStatusInvalidArgument, "failure arguments are invalid");
    }
    if(!manager->has_rollback_failure) {
        return astra_result_error(AstraStatusNotFound, "rollback failure not found");
    }
    *failure = manager->last_rollback_failure;
    return astra_result_ok();
}
