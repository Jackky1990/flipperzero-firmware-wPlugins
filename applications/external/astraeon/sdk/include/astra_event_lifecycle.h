#pragma once

#include "astra_event.h"
#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

AstraResult astra_event_lifecycle_init(void);
AstraResult astra_event_lifecycle_mark_dispatching(AstraEvent* event);
AstraResult astra_event_lifecycle_mark_completed(AstraEvent* event);
AstraResult astra_event_lifecycle_mark_failed(AstraEvent* event);

#ifdef __cplusplus
}
#endif
