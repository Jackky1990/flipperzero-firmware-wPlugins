#pragma once

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

AstraResult astra_runtime_loop_init(void);
AstraResult astra_runtime_loop_step(void);
AstraResult astra_runtime_loop_shutdown(void);

#ifdef __cplusplus
}
#endif
