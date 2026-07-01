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
    AstraFlipperGPIOAdapter gpio_adapter;
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

    if(astra_flipper_gpio_adapter_restore_if_needed(&gpio_adapter, AstraFlipperGPIOPinPC0).status !=
       AstraStatusNotFound) {
        return false;
    }

    return astra_device_usb_validate(&usb).status == AstraStatusOk &&
           astra_device_serial_validate(&serial).status == AstraStatusOk &&
           astra_device_ble_status_validate(&ble).status == AstraStatusOk &&
           astra_device_nfc_validate(&nfc).status == AstraStatusOk &&
           astra_device_rfid_validate(&rfid).status == AstraStatusOk &&
           astra_device_ir_validate(&ir).status == AstraStatusOk &&
           astra_device_subghz_validate(&subghz).status == AstraStatusOk;
}
