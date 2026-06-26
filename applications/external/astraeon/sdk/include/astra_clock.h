#pragma once

#include "astra_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize runtime clock.
 */
void astra_clock_init(void);

/**
 * Returns monotonic runtime timestamp.
 */
AstraTimestamp astra_clock_now(void);

/**
 * Returns elapsed time since the supplied timestamp.
 */
AstraTimestamp astra_clock_elapsed(AstraTimestamp start);

#ifdef __cplusplus
}
#endif
