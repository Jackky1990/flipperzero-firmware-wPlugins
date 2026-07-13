#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASTRA_SERVICE_MANAGER_MAX_SERVICES 8
#define ASTRA_SERVICE_MAX_DEPENDENCIES 4
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

typedef enum {
    AstraServiceRecoveryNoRetry = 0,
    AstraServiceRecoveryRetryOnce,
    AstraServiceRecoveryContinueAllowed,
} AstraServiceRecoveryPolicy;

typedef AstraResult (*AstraServiceLifecycleCallback)(void* context);
typedef AstraServiceHealth (*AstraServiceHealthCallback)(void* context);

typedef struct {
    const char* name;
    AstraServiceLifecycleCallback start;
    AstraServiceLifecycleCallback stop;
    AstraServiceHealthCallback health;
    void* context;
    const char* const* dependencies;
    size_t dependency_count;
} AstraServiceDefinition;

typedef struct {
    char name[ASTRA_SERVICE_NAME_MAX_LENGTH + 1];
    AstraServiceLifecycleCallback start;
    AstraServiceLifecycleCallback stop;
    AstraServiceHealthCallback health;
    void* context;
    char dependencies[ASTRA_SERVICE_MAX_DEPENDENCIES][ASTRA_SERVICE_NAME_MAX_LENGTH + 1];
    size_t dependency_count;
    AstraServiceState state;
    bool used;
} AstraService;

typedef struct {
    AstraService services[ASTRA_SERVICE_MANAGER_MAX_SERVICES];
    size_t start_order[ASTRA_SERVICE_MANAGER_MAX_SERVICES];
    size_t started_count;
    AstraResult last_start_failure;
    AstraResult last_rollback_failure;
    size_t failed_service_index;
    bool has_start_failure;
    bool has_rollback_failure;
    bool recovery_attempted;
} AstraServiceManager;

AstraResult astra_service_manager_init(void);
AstraResult astra_service_manager_register(const AstraServiceDefinition* definition);
AstraResult astra_service_manager_start(const char* name);
AstraResult astra_service_manager_stop(const char* name);
const AstraService* astra_service_manager_find(const char* name);
AstraResult astra_service_manager_health(const char* name, AstraServiceHealth* health);
AstraResult astra_service_manager_validate(void);
AstraResult astra_service_manager_start_all(void);
AstraResult astra_service_manager_shutdown(void);
AstraResult astra_service_manager_health_aggregate(AstraServiceHealth* health);
AstraResult astra_service_manager_recover(AstraServiceRecoveryPolicy policy);
AstraResult astra_service_manager_last_start_failure(AstraResult* failure);
AstraResult astra_service_manager_last_rollback_failure(AstraResult* failure);

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
AstraResult astra_service_manager_validate_context(struct AstraRuntimeContext* context);
AstraResult astra_service_manager_start_all_context(struct AstraRuntimeContext* context);
AstraResult astra_service_manager_shutdown_context(struct AstraRuntimeContext* context);
AstraResult astra_service_manager_health_aggregate_context(
    struct AstraRuntimeContext* context,
    AstraServiceHealth* health);
AstraResult astra_service_manager_recover_context(
    struct AstraRuntimeContext* context,
    AstraServiceRecoveryPolicy policy);
AstraResult astra_service_manager_last_start_failure_context(
    struct AstraRuntimeContext* context,
    AstraResult* failure);
AstraResult astra_service_manager_last_rollback_failure_context(
    struct AstraRuntimeContext* context,
    AstraResult* failure);

#ifdef __cplusplus
}
#endif
