#pragma once

#include "astra_result.h"
#include "astra_event.h"

#ifdef __cplusplus
extern "C" {
#endif

AstraResult astra_dispatcher_init(void);
AstraResult astra_dispatcher_dispatch(AstraEvent* event);

#ifdef __cplusplus
}
#endif
