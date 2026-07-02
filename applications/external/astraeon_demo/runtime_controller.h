#pragma once

#include "context/runtime_context.h"

#include "astra_device_serial.h"
#include "astra_flipper_serial_adapter.h"

#include <storage/storage.h>

void astraeon_demo_runtime_controller_start(AstraeonRuntimeContext* runtime, Storage* storage);

void astraeon_demo_runtime_controller_validate_gpio_read(
    AstraeonRuntimeContext* runtime,
    Storage* storage);

AstraStatus astraeon_demo_runtime_controller_open_uart(
    AstraeonRuntimeContext* runtime,
    AstraFlipperSerialAdapter* adapter,
    Storage* storage,
    const AstraDeviceSerialConfig* config);

AstraStatus astraeon_demo_runtime_controller_close_uart(
    AstraeonRuntimeContext* runtime,
    AstraFlipperSerialAdapter* adapter,
    Storage* storage);

AstraStatus astraeon_demo_runtime_controller_cancel_uart(
    AstraeonRuntimeContext* runtime,
    AstraFlipperSerialAdapter* adapter,
    Storage* storage);

AstraStatus astraeon_demo_runtime_controller_timeout_uart(
    AstraeonRuntimeContext* runtime,
    AstraFlipperSerialAdapter* adapter,
    Storage* storage);
