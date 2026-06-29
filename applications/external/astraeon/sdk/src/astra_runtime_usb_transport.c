#include "astra_runtime_usb_transport.h"

#if defined(ASTRA_RUNTIME_ENABLE_USB_CDC)
#include <furi_hal_usb_cdc.h>
#define ASTRA_USB_CDC_CHANNEL 0u
#endif

static AstraResult astra_usb_transport_send(const uint8_t* data, size_t size) {
    if(!data || size == 0) {
        return astra_result_error(AstraStatusInvalidArgument, "usb send invalid");
    }

#if defined(ASTRA_RUNTIME_ENABLE_USB_CDC)
    if(size > 64) {
        return astra_result_error(AstraStatusBusy, "usb payload too large");
    }

    furi_hal_cdc_send(ASTRA_USB_CDC_CHANNEL, (uint8_t*)data, (uint16_t)size);
    return astra_result_ok();
#else
    return astra_result_error(AstraStatusNotFound, "usb transport not attached");
#endif
}

static AstraResult astra_usb_transport_receive(uint8_t* buffer, size_t buffer_size, size_t* received_size) {
    if(!buffer || buffer_size == 0 || !received_size) {
        return astra_result_error(AstraStatusInvalidArgument, "usb receive invalid");
    }

    *received_size = 0;

#if defined(ASTRA_RUNTIME_ENABLE_USB_CDC)
    uint16_t max_len = buffer_size > 64 ? 64 : (uint16_t)buffer_size;
    int32_t received = furi_hal_cdc_receive(ASTRA_USB_CDC_CHANNEL, buffer, max_len);

    if(received <= 0) {
        return astra_result_error(AstraStatusNotFound, "usb no data");
    }

    *received_size = (size_t)received;
    return astra_result_ok();
#else
    return astra_result_error(AstraStatusNotFound, "usb transport not attached");
#endif
}

AstraResult astra_runtime_usb_transport_init(AstraRuntimeTransport* transport) {
    if(!transport) {
        return astra_result_error(AstraStatusInvalidArgument, "transport is null");
    }

    transport->send = astra_usb_transport_send;
    transport->receive = astra_usb_transport_receive;
    transport->capabilities.supports_stream = true;
    transport->capabilities.supports_packets = false;
    transport->capabilities.reliable = true;
    transport->capabilities.max_payload_size = 64;
    return astra_result_ok();
}
