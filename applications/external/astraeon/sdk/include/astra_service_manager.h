#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASTRA_SERVICE_MANAGER_MAX_SERVICES 8
#define ASTRA_SERVICE_NAME_MAX_LENGTH 31

struct AstraRuntimeContext;

typedef enum {
    AstraServiceStateStopped = 0,
    AstraServiceStateRunning,
} AstraServiceState;

typedef enum {
    AstraServiceHealthUnknown = 0,
    AstraServiceHealthStopped,
    AstraServiceHealthHealthy,
    AstraServiceHealthDegraded,
    AstraServiceHealthUnhealthy,
} AstraServiceHealth;

typedef AstraResult (*AstraServiceLifecycleCallback)(void* context);
typedef AstraServiceHealth (*AstraServiceHealthCallback)(void* context);

typedef struct {
    const char* name;
    AstraServiceLifecycleCallback start;
    AstraServiceLifecycleCallback stop;
    AstraServiceHealthCallback health;
    void* context;
} AstraServiceDefinition;

typedef struct {
    char name[ASTRA_SERVICE_NAME_MAX_LENGTH + 1];
    AstraServiceLifecycleCallback start;
    AstraServiceLifecycleCallback stop;
    AstraServiceHealthCallback health;
    void* context;
    AstraServiceState state;
    bool used;
} AstraService;

typedef struct {
    AstraService services[ASTRA_SERVICE_MANAGER_MAX_SERVICES];
} AstraServiceManager;

AstraResult astra_service_manager_init(void);
AstraResult astra_service_manager_register(const AstraServiceDefinition* definition);
AstraResult astra_service_manager_start(const char* name);
AstraResult astra_service_manager_stop(const char* name);
const AstraService* astra_service_manager_find(const char* name);
AstraResult astra_service_manager_health(const char* name, AstraServiceHealth* health);

AstraResult astra_service_manager_init_context(struct AstraRuntimeContext* context);
AstraResult astra_service_manager_register_context(
    struct AstraRuntimeContext* context,
    const AstraServiceDefinition* definition);
AstraResult astra_service_manager_start_context(
    struct AstraRuntimeContext* context,
    const char* name);
AstraResult astra_service_manager_stop_context(
    struct AstraRuntimeContext* context,
    const char* name);
const AstraService* astra_service_manager_find_context(
    struct AstraRuntimeContext* context,
    const char* name);
AstraResult astra_service_manager_health_context(
    struct AstraRuntimeContext* context,
    const char* name,
    AstraServiceHealth* health);

#ifdef __cplusplus
}
#endif
