#pragma once

#include "context/runtime_context.h"

#include <storage/storage.h>

void astraeon_demo_runtime_controller_start(AstraeonRuntimeContext* runtime, Storage* storage);

void astraeon_demo_runtime_controller_validate_gpio_read(
    AstraeonRuntimeContext* runtime,
    Storage* storage);
