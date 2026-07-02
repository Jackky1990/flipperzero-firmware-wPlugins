#include "astra_device.h"
#include "astra_device_ble.h"
#include "astra_device_gpio.h"
#include "astra_device_ir.h"
#include "astra_device_nfc.h"
#include "astra_device_rfid.h"
#include "astra_device_serial.h"
#include "astra_device_subghz.h"
#include "astra_device_usb.h"
#include "astra_tests.h"
#include "../../sdk/src/astra_flipper_gpio_adapter.h"
#include "../../../astraeon_demo/context/runtime_context.h"

bool astra_test_device(void) {
    AstraDevice device;
    AstraDeviceCapabilities capabilities;
    AstraDeviceGPIOPin pin = {0};
    AstraDeviceUSB usb = {0};
    AstraDeviceSerial serial = {0};
    AstraDeviceBLEStatus ble = {0};
    AstraDeviceNFC nfc = {0};
    AstraDeviceRFID rfid = {0};
    AstraDeviceIR ir = {0};
    AstraDeviceSubGhz subghz = {0};
    AstraDeviceSerialConfig serial_config = {0};
    AstraDeviceSerialStatus serial_status = {0};
    AstraDeviceSerialDiagnostics serial_diagnostics = {0};
    AstraDeviceSerialSession serial_session = {0};
    AstraFlipperGPIOAdapter gpio_adapter;
    AstraeonRuntimeContext gpio_write_runtime = {0};
    bool gpio_value = false;
    bool gpio_mode_changed = true;

    if(astra_device_init(0, "flipper").status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_device_init(&device, "").status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_device_init(&device, "flipper-zero").status != AstraStatusOk) {
        return false;
    }

    if(astra_device_validate(&device).status != AstraStatusOk) {
        return false;
    }

    if(astra_device_capabilities_init(&capabilities).status != AstraStatusOk) {
        return false;
    }

    capabilities.feature_flags = ASTRA_DEVICE_CAPABILITY_GPIO | ASTRA_DEVICE_CAPABILITY_USB;
    if(!astra_device_capabilities_supports(&capabilities, ASTRA_DEVICE_CAPABILITY_GPIO)) {
        return false;
    }

    if(astra_device_set_capabilities(&device, &capabilities).status != AstraStatusOk) {
        return false;
    }

    if(!astra_device_capabilities(&device)) {
        return false;
    }

    pin.mode = AstraDeviceGPIOModeOutput;
    if(astra_device_gpio_pin_validate(&pin).status != AstraStatusOk) {
        return false;
    }

    usb.supports_cdc = true;
    if(astra_device_usb_validate(&usb).status != AstraStatusInvalidArgument) {
        return false;
    }
    usb.max_packet_size = 64;

    serial.baud_rate = 115200;
    serial_config.channel = AstraDeviceSerialChannelPrimary;
    serial_config.baud_rate = 115200;
    serial_config.data_bits = 8;
    serial_config.parity = AstraDeviceSerialParityNone;
    serial_config.stop_bits = AstraDeviceSerialStopBits1;
    serial_config.flow_control = AstraDeviceSerialFlowControlNone;
    serial_config.rx_buffer_size = 256;
    serial_config.tx_buffer_size = 256;
    serial_config.timeout_ms = 100;
    serial_status.open = true;
    serial_status.last_status = AstraStatusOk;
    serial_session.session_id = 1;
    serial_session.active = true;
    serial_session.config = serial_config;
    serial_session.status = serial_status;
    serial_session.diagnostics = serial_diagnostics;
    ble.advertising = true;
    nfc.supports_poller = true;
    rfid.supports_read = true;
    ir.supports_rx = true;
    subghz.supports_rx = true;
    subghz.min_frequency_hz = 300000000;
    subghz.max_frequency_hz = 928000000;

    if(astra_flipper_gpio_adapter_init(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_flipper_gpio_adapter_bind_resources(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_flipper_gpio_adapter_init(&gpio_adapter).status != AstraStatusOk) {
        return false;
    }

    if(astra_flipper_gpio_adapter_validate(&gpio_adapter).status != AstraStatusOk) {
        return false;
    }

    if(astra_flipper_gpio_adapter_pin_count(&gpio_adapter) != AstraFlipperGPIOPinCount) {
        return false;
    }

    if(!astra_flipper_gpio_adapter_pin_at(&gpio_adapter, AstraFlipperGPIOPinPA7)) {
        return false;
    }

    if(astra_flipper_gpio_adapter_pin_at(&gpio_adapter, AstraFlipperGPIOPinCount)) {
        return false;
    }

    if(astra_flipper_gpio_adapter_read_pin(0, AstraFlipperGPIOPinPC0, &gpio_value).status !=
       AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_flipper_gpio_adapter_set_input_mode(0, AstraFlipperGPIOPinPC0, &gpio_mode_changed)
           .status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_flipper_gpio_adapter_set_input_mode(&gpio_adapter, AstraFlipperGPIOPinPC0, 0).status !=
       AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_flipper_gpio_adapter_set_output_low_mode(0, AstraFlipperGPIOPinPC0, &gpio_mode_changed)
           .status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_flipper_gpio_adapter_set_output_low_mode(&gpio_adapter, AstraFlipperGPIOPinPC0, 0)
           .status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_flipper_gpio_adapter_read_pin(&gpio_adapter, AstraFlipperGPIOPinPC0, 0).status !=
       AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_flipper_gpio_adapter_read_pin(
           &gpio_adapter,
           AstraFlipperGPIOPinCount,
           &gpio_value)
           .status != AstraStatusInvalidArgument) {
        return false;
    }

    gpio_mode_changed = true;
    if(astra_flipper_gpio_adapter_set_input_mode(
           &gpio_adapter,
           AstraFlipperGPIOPinCount,
           &gpio_mode_changed)
           .status != AstraStatusInvalidArgument) {
        return false;
    }

    if(gpio_mode_changed) {
        return false;
    }

    gpio_mode_changed = true;
    if(astra_flipper_gpio_adapter_set_output_low_mode(
           &gpio_adapter,
           AstraFlipperGPIOPinCount,
           &gpio_mode_changed)
           .status != AstraStatusInvalidArgument) {
        return false;
    }

    if(gpio_mode_changed) {
        return false;
    }

    if(astra_flipper_gpio_adapter_read_pin(
           &gpio_adapter,
           AstraFlipperGPIOPinPC0,
           &gpio_value)
           .status != AstraStatusNotFound) {
        return false;
    }

    gpio_mode_changed = true;
    if(astra_flipper_gpio_adapter_set_input_mode(
           &gpio_adapter,
           AstraFlipperGPIOPinPC0,
           &gpio_mode_changed)
           .status != AstraStatusNotFound) {
        return false;
    }

    if(gpio_mode_changed) {
        return false;
    }

    gpio_mode_changed = true;
    if(astra_flipper_gpio_adapter_set_output_low_mode(
           &gpio_adapter,
           AstraFlipperGPIOPinPC0,
           &gpio_mode_changed)
           .status != AstraStatusNotFound) {
        return false;
    }

    if(gpio_mode_changed) {
        return false;
    }

    if(astra_flipper_gpio_adapter_write_pin(0, AstraFlipperGPIOPinPC0, false).status !=
       AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_flipper_gpio_adapter_write_pin(
           &gpio_adapter,
           AstraFlipperGPIOPinCount,
           false)
           .status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_flipper_gpio_adapter_write_pin(&gpio_adapter, AstraFlipperGPIOPinPC0, false).status !=
       AstraStatusNotFound) {
        return false;
    }

    if(astra_flipper_gpio_adapter_restore_if_needed(&gpio_adapter, AstraFlipperGPIOPinPC0).status !=
       AstraStatusNotFound) {
        return false;
    }

    if(astraeon_runtime_gpio_write_request_confirmation(0) != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_device_serial_validate(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    serial.baud_rate = 0;
    if(astra_device_serial_validate(&serial).status != AstraStatusInvalidArgument) {
        return false;
    }
    serial.baud_rate = 115200;

    if(astra_device_serial_config_validate(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    serial_config.baud_rate = ASTRA_DEVICE_SERIAL_MIN_BAUD_RATE - 1u;
    if(astra_device_serial_config_validate(&serial_config).status !=
       AstraStatusInvalidArgument) {
        return false;
    }
    serial_config.baud_rate = 115200;

    serial_config.channel = AstraDeviceSerialChannelCount;
    if(astra_device_serial_config_validate(&serial_config).status !=
       AstraStatusInvalidArgument) {
        return false;
    }
    serial_config.channel = AstraDeviceSerialChannelPrimary;

    serial_config.data_bits = 9;
    if(astra_device_serial_config_validate(&serial_config).status !=
       AstraStatusInvalidArgument) {
        return false;
    }
    serial_config.data_bits = 8;

    serial_config.rx_buffer_size = 0;
    if(astra_device_serial_config_validate(&serial_config).status !=
       AstraStatusInvalidArgument) {
        return false;
    }
    serial_config.rx_buffer_size = 256;

    if(astra_device_serial_status_validate(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    serial_status.open = false;
    serial_status.busy = true;
    if(astra_device_serial_status_validate(&serial_status).status !=
       AstraStatusInvalidArgument) {
        return false;
    }
    serial_status.open = true;
    serial_status.busy = false;

    if(astra_device_serial_diagnostics_validate(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_device_serial_session_validate(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    serial_session.session_id = 0;
    if(astra_device_serial_session_validate(&serial_session).status !=
       AstraStatusInvalidArgument) {
        return false;
    }
    serial_session.session_id = 1;

    if(astraeon_runtime_gpio_write_request_confirmation(&gpio_write_runtime) != AstraStatusOk) {
        return false;
    }

    if(!gpio_write_runtime.gpio_write_confirm_required ||
       gpio_write_runtime.gpio_write_state != AstraeonGPIOWriteStateConfirmRequired) {
        return false;
    }

    if(astraeon_runtime_gpio_write_begin(&gpio_write_runtime) != AstraStatusOk) {
        return false;
    }

    if(!gpio_write_runtime.gpio_write_session_active ||
       gpio_write_runtime.gpio_write_confirm_required ||
       gpio_write_runtime.gpio_write_runs != 1 ||
       gpio_write_runtime.gpio_write_state != AstraeonGPIOWriteStateActive) {
        return false;
    }

    if(astraeon_runtime_gpio_write_begin(&gpio_write_runtime) != AstraStatusBusy) {
        return false;
    }

    astraeon_runtime_gpio_write_finish(&gpio_write_runtime, AstraStatusOk);
    if(gpio_write_runtime.gpio_write_session_active || !gpio_write_runtime.gpio_write_ok ||
       gpio_write_runtime.gpio_write_status != AstraStatusOk) {
        return false;
    }

    gpio_write_runtime = (AstraeonRuntimeContext){0};
    astraeon_runtime_gpio_write_request_confirmation(&gpio_write_runtime);
    if(astraeon_runtime_gpio_write_cancel(&gpio_write_runtime) != AstraStatusPolicyDenied ||
       gpio_write_runtime.gpio_write_confirm_required ||
       gpio_write_runtime.gpio_write_status != AstraStatusPolicyDenied) {
        return false;
    }

    gpio_write_runtime = (AstraeonRuntimeContext){0};
    astraeon_runtime_gpio_write_request_confirmation(&gpio_write_runtime);
    if(astraeon_runtime_gpio_write_timeout(&gpio_write_runtime) != AstraStatusTimeout ||
       gpio_write_runtime.gpio_write_confirm_required ||
       gpio_write_runtime.gpio_write_status != AstraStatusTimeout) {
        return false;
    }

    gpio_write_runtime = (AstraeonRuntimeContext){0};
    astraeon_runtime_gpio_write_request_confirmation(&gpio_write_runtime);
    astraeon_runtime_gpio_write_begin(&gpio_write_runtime);
    astraeon_runtime_gpio_write_set_mode_changed(&gpio_write_runtime, true);
    astraeon_runtime_gpio_write_mark_restore(&gpio_write_runtime);
    astraeon_runtime_gpio_write_mark_restore(&gpio_write_runtime);
    if(!gpio_write_runtime.gpio_write_restored ||
       gpio_write_runtime.gpio_write_restore_count != 1) {
        return false;
    }

    astraeon_runtime_gpio_write_finish(&gpio_write_runtime, AstraStatusOk);
    if(!gpio_write_runtime.gpio_write_checked || !gpio_write_runtime.gpio_write_ok ||
       gpio_write_runtime.gpio_write_state != AstraeonGPIOWriteStateCompleted) {
        return false;
    }

    return astra_device_usb_validate(&usb).status == AstraStatusOk &&
           astra_device_serial_validate(&serial).status == AstraStatusOk &&
           astra_device_serial_config_validate(&serial_config).status == AstraStatusOk &&
           astra_device_serial_status_validate(&serial_status).status == AstraStatusOk &&
           astra_device_serial_diagnostics_validate(&serial_diagnostics).status == AstraStatusOk &&
           astra_device_serial_session_validate(&serial_session).status == AstraStatusOk &&
           astra_device_ble_status_validate(&ble).status == AstraStatusOk &&
           astra_device_nfc_validate(&nfc).status == AstraStatusOk &&
           astra_device_rfid_validate(&rfid).status == AstraStatusOk &&
           astra_device_ir_validate(&ir).status == AstraStatusOk &&
           astra_device_subghz_validate(&subghz).status == AstraStatusOk;
}
