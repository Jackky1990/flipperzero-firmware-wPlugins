#include "astra_runtime_manager.h"
#include "astra_runtime_default.h"

#include <stdbool.h>
#include <stddef.h>

#define ASTRA_RUNTIME_MANAGER_MAX_RUNTIMES 4

typedef struct {
    AstraRuntimeContext context;
    bool used;
} AstraRuntimeSlot;

static AstraRuntimeSlot runtimes[ASTRA_RUNTIME_MANAGER_MAX_RUNTIMES];

AstraResult astra_runtime_manager_init(void) {
    for(size_t i = 0; i < ASTRA_RUNTIME_MANAGER_MAX_RUNTIMES; i++) {
        runtimes[i].used = false;
        astra_runtime_context_reset(&runtimes[i].context);
    }

    runtimes[0].used = true;
    astra_runtime_default_init();
    runtimes[0].context = *astra_runtime_default_context();

    return astra_result_ok();
}

AstraResult astra_runtime_create(
    AstraRuntimeId* id,
    AstraRuntimeContext** context) {
    if(!id || !context) {
        return astra_result_error(AstraStatusInvalidArgument, "output is null");
    }

    for(size_t i = 1; i < ASTRA_RUNTIME_MANAGER_MAX_RUNTIMES; i++) {
        if(!runtimes[i].used) {
            runtimes[i].used = true;

            AstraResult result = astra_runtime_context_init(&runtimes[i].context);
            if(result.status != AstraStatusOk) {
                runtimes[i].used = false;
                return result;
            }

            *id = (AstraRuntimeId)i;
            *context = &runtimes[i].context;
            return astra_result_ok();
        }
    }

    return astra_result_error(AstraStatusBusy, "runtime manager is full");
}

AstraResult astra_runtime_destroy(AstraRuntimeId id) {
    if(id == 0 || id >= ASTRA_RUNTIME_MANAGER_MAX_RUNTIMES || !runtimes[id].used) {
        return astra_result_error(AstraStatusNotFound, "runtime not found");
    }

    astra_runtime_context_reset(&runtimes[id].context);
    runtimes[id].used = false;
    return astra_result_ok();
}

AstraRuntimeContext* astra_runtime_get(AstraRuntimeId id) {
    if(id >= ASTRA_RUNTIME_MANAGER_MAX_RUNTIMES || !runtimes[id].used) {
        return 0;
    }

    return &runtimes[id].context;
}

AstraRuntimeContext* astra_runtime_default(void) {
    return astra_runtime_default_context();
}

size_t astra_runtime_count(void) {
    size_t count = 0;

    for(size_t i = 0; i < ASTRA_RUNTIME_MANAGER_MAX_RUNTIMES; i++) {
        if(runtimes[i].used) {
            count++;
        }
    }

    return count;
}

bool astra_runtime_exists(AstraRuntimeId id) {
    return astra_runtime_get(id) != 0;
}
