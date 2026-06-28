#pragma once

#include <stdint.h>

#include "astra_result.h"
#include "astra_runtime_context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t AstraRuntimeId;

AstraResult astra_runtime_manager_init(void);

AstraResult astra_runtime_create(
    AstraRuntimeId* id,
    AstraRuntimeContext** context);

AstraResult astra_runtime_destroy(AstraRuntimeId id);

AstraRuntimeContext* astra_runtime_get(AstraRuntimeId id);

AstraRuntimeContext* astra_runtime_default(void);

#ifdef __cplusplus
}
#endif
