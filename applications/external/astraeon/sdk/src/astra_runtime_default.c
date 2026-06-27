#include "astra_runtime_default.h"

static AstraRuntimeContext default_context;

AstraRuntimeContext* astra_runtime_default_context(void) {
    return &default_context;
}

AstraResult astra_runtime_default_init(void) {
    return astra_runtime_context_init(&default_context);
}

AstraResult astra_runtime_default_reset(void) {
    return astra_runtime_context_reset(&default_context);
}
