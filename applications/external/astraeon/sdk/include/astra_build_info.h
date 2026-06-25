#pragma once

#include "astra_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASTRA_BUILD_NAME "ASTRAEON Edge Platform"
#define ASTRA_BUILD_TARGET "Flipper Zero RogueMaster"
#define ASTRA_MIN_ROGUEMASTER_VERSION "RM420"
#define ASTRA_BUILD_CHANNEL "alpha"

const char* astra_build_name(void);
const char* astra_build_target(void);
const char* astra_build_channel(void);
const char* astra_build_min_roguemaster_version(void);
AstraVersion astra_build_aep_version(void);
AstraVersion astra_build_protocol_version(void);

#ifdef __cplusplus
}
#endif
