#include "astra_registry.h"
#include <stdbool.h>
#include <string.h>

#define ASTRA_REGISTRY_MAX_ENTRIES 16

typedef struct {
    AstraEventType type;
    AstraEventHandler handler;
    bool used;
} AstraRegistryEntry;

static AstraRegistryEntry registry[ASTRA_REGISTRY_MAX_ENTRIES];

AstraResult astra_registry_init(void) {
    memset(registry, 0, sizeof(registry));
    return astra_result_ok();
}

AstraResult astra_registry_register(AstraEventType type, AstraEventHandler handler) {
    if(!handler) {
        return astra_result_error(AstraStatusInvalidArgument, "handler is null");
    }

    for(size_t index = 0; index < ASTRA_REGISTRY_MAX_ENTRIES; index++) {
        if(registry[index].used && registry[index].type == type) {
            registry[index].handler = handler;
            return astra_result_ok();
        }
    }

    for(size_t index = 0; index < ASTRA_REGISTRY_MAX_ENTRIES; index++) {
        if(!registry[index].used) {
            registry[index].used = true;
            registry[index].type = type;
            registry[index].handler = handler;
            return astra_result_ok();
        }
    }

    return astra_result_error(AstraStatusBusy, "registry is full");
}

AstraEventHandler astra_registry_find(AstraEventType type) {
    for(size_t index = 0; index < ASTRA_REGISTRY_MAX_ENTRIES; index++) {
        if(registry[index].used && registry[index].type == type) {
            return registry[index].handler;
        }
    }

    return 0;
}
