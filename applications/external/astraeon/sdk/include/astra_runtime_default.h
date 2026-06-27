#pragma once

#include "astra_result.h"
#include "astra_runtime_context.h"

#ifdef __cplusplus
extern "C" {
#endif

AstraRuntimeContext* astra_runtime_default_context(void);
AstraResult astra_runtime_default_init(void);
AstraResult astra_runtime_default_reset(void);

#ifdef __cplusplus
}
#endif
