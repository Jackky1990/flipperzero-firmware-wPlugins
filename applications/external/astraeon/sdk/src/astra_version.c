#include "astra_version.h"
#include <stdio.h>

AstraVersion astra_version_aep(void) {
    AstraVersion version = {
        .major = ASTRA_AEP_VERSION_MAJOR,
        .minor = ASTRA_AEP_VERSION_MINOR,
        .patch = ASTRA_AEP_VERSION_PATCH,
    };
    return version;
}

AstraVersion astra_version_protocol(void) {
    AstraVersion version = {
        .major = ASTRA_PROTOCOL_VERSION_MAJOR,
        .minor = ASTRA_PROTOCOL_VERSION_MINOR,
        .patch = ASTRA_PROTOCOL_VERSION_PATCH,
    };
    return version;
}

void astra_version_to_string(AstraVersion version, char* buffer, unsigned int buffer_size) {
    if(!buffer || buffer_size == 0) {
        return;
    }
    snprintf(buffer, buffer_size, "%u.%u.%u", version.major, version.minor, version.patch);
}
