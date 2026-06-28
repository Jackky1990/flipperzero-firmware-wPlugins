#include "astra_tests.h"
#include "astra_runtime_codec.h"

bool astra_test_runtime_codec(void) {
    return astra_runtime_codec_init().status == AstraStatusOk;
}
