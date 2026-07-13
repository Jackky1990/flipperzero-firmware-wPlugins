#include "astra_service_manager.h"

#include <string.h>

#include "astra_runtime_context.h"
#include "astra_runtime_default.h"

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
    AstraService* service = astra_service_manager_find_mutable(
        astra_service_manager_from_context(context),
        name);
    if(!service) {
        return astra_result_error(AstraStatusNotFound, "service not found");
    }
    if(service->state == AstraServiceStateRunning) {
        return astra_result_error(AstraStatusBusy, "service is already running");
    }

    AstraResult result = service->start(service->context);
    if(result.status == AstraStatusOk) {
        service->state = AstraServiceStateRunning;
    }
    return result;
}

AstraResult astra_service_manager_stop_context(AstraRuntimeContext* context, const char* name) {
    AstraResult validation = astra_service_manager_validate_lookup(context, name);
    if(validation.status != AstraStatusOk) {
        return validation;
    }
    AstraService* service = astra_service_manager_find_mutable(
        astra_service_manager_from_context(context),
        name);
    if(!service) {
        return astra_result_error(AstraStatusNotFound, "service not found");
    }
    if(service->state == AstraServiceStateStopped) {
        return astra_result_error(AstraStatusBusy, "service is already stopped");
    }

    AstraResult result = service->stop(service->context);
    if(result.status == AstraStatusOk) {
        service->state = AstraServiceStateStopped;
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

    AstraService* service = astra_service_manager_find_mutable(
        astra_service_manager_from_context(context),
        name);
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
