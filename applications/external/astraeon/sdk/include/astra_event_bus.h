#pragma once

#include "astra_result.h"
#include "astra_event.h"

#ifdef __cplusplus
extern "C" {
#endif

AstraResult astra_event_bus_init(void);
AstraResult astra_event_bus_publish(AstraEvent* event);

#ifdef __cplusplus
}
#endif
