#pragma once

#include "astra_result.h"
#include "astra_event.h"

#ifdef __cplusplus
extern "C" {
#endif

AstraResult astra_scheduler_init(void);
AstraResult astra_scheduler_schedule(const AstraEvent* event);
AstraResult astra_scheduler_step(void);

#ifdef __cplusplus
}
#endif
