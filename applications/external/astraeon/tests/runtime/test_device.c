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

    return astra_device_usb_validate(&usb).status == AstraStatusOk &&
           astra_device_serial_validate(&serial).status == AstraStatusOk &&
           astra_device_ble_status_validate(&ble).status == AstraStatusOk &&
           astra_device_nfc_validate(&nfc).status == AstraStatusOk &&
           astra_device_rfid_validate(&rfid).status == AstraStatusOk &&
           astra_device_ir_validate(&ir).status == AstraStatusOk &&
           astra_device_subghz_validate(&subghz).status == AstraStatusOk;
}
