#pragma once

#include "context/runtime_context.h"

#include "astra_device_serial.h"
#include "astra_flipper_serial_adapter.h"

#include <storage/storage.h>
#include <stddef.h>

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

AstraStatus astraeon_demo_runtime_controller_uart_tx(
    AstraeonRuntimeContext* runtime,
    AstraFlipperSerialAdapter* adapter,
    Storage* storage,
    const uint8_t* data,
    size_t length,
    size_t* out_written);

AstraStatus astraeon_demo_runtime_controller_uart_rx_arm(
    AstraeonRuntimeContext* runtime,
    AstraFlipperSerialAdapter* adapter,
    Storage* storage,
    size_t expected_length);

AstraStatus astraeon_demo_runtime_controller_uart_rx_drain(
    AstraeonRuntimeContext* runtime,
    AstraFlipperSerialAdapter* adapter,
    Storage* storage,
    const uint8_t* expected_data,
    size_t expected_length,
    size_t* out_read);

AstraStatus astraeon_demo_runtime_controller_uart_rx_cancel(
    AstraeonRuntimeContext* runtime,
    AstraFlipperSerialAdapter* adapter,
    Storage* storage);

AstraStatus astraeon_demo_runtime_controller_uart_rx_timeout(
    AstraeonRuntimeContext* runtime,
    AstraFlipperSerialAdapter* adapter,
    Storage* storage);
