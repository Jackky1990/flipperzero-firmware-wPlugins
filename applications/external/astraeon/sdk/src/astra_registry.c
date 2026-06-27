#include "astra_registry.h"
#include <stdbool.h>
#include <string.h>

#define ASTRA_REGISTRY_MAX_ENTRIES 16

typedef struct {
    AstraEventType type;
    AstraEventHandler handler;
    bool used;
} AstraRegistryEntry;

typedef struct {
    AstraRegistryEntry entries[ASTRA_REGISTRY_MAX_ENTRIES];
} AstraRegistryStore;

static AstraRegistryStore default_registry;

static AstraRegistryStore* astra_registry_store_from_context(AstraRuntimeContext* context) {
    if(!context) {
        return 0;
    }

    return (AstraRegistryStore*)astra_runtime_context_get_registry(context);
}

static void astra_registry_store_reset(AstraRegistryStore* store) {
    memset(store, 0, sizeof(*store));
}

static AstraResult astra_registry_store_register(
    AstraRegistryStore* store,
    AstraEventType type,
    AstraEventHandler handler) {

    if(!store) {
        return astra_result_error(AstraStatusInvalidArgument, "registry store is null");
    }

    if(!handler) {
        return astra_result_error(AstraStatusInvalidArgument, "handler is null");
    }

    for(size_t index = 0; index < ASTRA_REGISTRY_MAX_ENTRIES; index++) {
        if(store->entries[index].used && store->entries[index].type == type) {
            store->entries[index].handler = handler;
            return astra_result_ok();
        }
    }

    for(size_t index = 0; index < ASTRA_REGISTRY_MAX_ENTRIES; index++) {
        if(!store->entries[index].used) {
            store->entries[index].used = true;
            store->entries[index].type = type;
            store->entries[index].handler = handler;
            return astra_result_ok();
        }
    }

    return astra_result_error(AstraStatusBusy, "registry is full");
}

static AstraEventHandler astra_registry_store_find(
    AstraRegistryStore* store,
    AstraEventType type) {

    if(!store) {
        return 0;
    }

    for(size_t index = 0; index < ASTRA_REGISTRY_MAX_ENTRIES; index++) {
        if(store->entries[index].used && store->entries[index].type == type) {
            return store->entries[index].handler;
        }
    }

    return 0;
}

AstraResult astra_registry_init(void) {
    astra_registry_store_reset(&default_registry);
    return astra_result_ok();
}

AstraResult astra_registry_register(AstraEventType type, AstraEventHandler handler) {
    return astra_registry_store_register(&default_registry, type, handler);
}

AstraEventHandler astra_registry_find(AstraEventType type) {
    return astra_registry_store_find(&default_registry, type);
}

AstraResult astra_registry_init_context(AstraRuntimeContext* context) {
    if(!context) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }

    static AstraRegistryStore context_registry;

    astra_registry_store_reset(&context_registry);

    AstraResult result = astra_runtime_context_set_registry(context, &context_registry);
    if(result.status != AstraStatusOk) {
        return result;
    }

    return astra_result_ok();
}

AstraResult astra_registry_register_context(
    AstraRuntimeContext* context,
    AstraEventType type,
    AstraEventHandler handler) {

    return astra_registry_store_register(
        astra_registry_store_from_context(context),
        type,
        handler);
}

AstraEventHandler astra_registry_find_context(
    AstraRuntimeContext* context,
    AstraEventType type) {

    return astra_registry_store_find(
        astra_registry_store_from_context(context),
        type);
}
