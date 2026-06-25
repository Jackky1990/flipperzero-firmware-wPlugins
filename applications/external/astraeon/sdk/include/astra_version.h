#pragma once

#include "astra_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASTRA_AEP_VERSION_MAJOR 0
#define ASTRA_AEP_VERSION_MINOR 1
#define ASTRA_AEP_VERSION_PATCH 0

#define ASTRA_PROTOCOL_VERSION_MAJOR 1
#define ASTRA_PROTOCOL_VERSION_MINOR 0
#define ASTRA_PROTOCOL_VERSION_PATCH 0

AstraVersion astra_version_aep(void);
AstraVersion astra_version_protocol(void);
void astra_version_to_string(AstraVersion version, char* buffer, unsigned int buffer_size);

#ifdef __cplusplus
}
#endif
