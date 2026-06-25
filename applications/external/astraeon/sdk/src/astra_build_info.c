#include "astra_build_info.h"
#include "astra_version.h"

const char* astra_build_name(void) {
    return ASTRA_BUILD_NAME;
}

const char* astra_build_target(void) {
    return ASTRA_BUILD_TARGET;
}

const char* astra_build_channel(void) {
    return ASTRA_BUILD_CHANNEL;
}

const char* astra_build_min_roguemaster_version(void) {
    return ASTRA_MIN_ROGUEMASTER_VERSION;
}

AstraVersion astra_build_aep_version(void) {
    return astra_version_aep();
}

AstraVersion astra_build_protocol_version(void) {
    return astra_version_protocol();
}
