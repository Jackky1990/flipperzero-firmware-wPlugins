#pragma once

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

AstraResult astra_runtime_init(void);
AstraResult astra_runtime_step(void);

#ifdef __cplusplus
}
#endif
