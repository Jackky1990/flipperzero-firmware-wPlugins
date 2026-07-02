#include "astra_device_serial.h"

AstraResult astra_device_serial_validate(const AstraDeviceSerial* serial) {
    if(!serial) {
        return astra_result_error(AstraStatusInvalidArgument, "serial is null");
    }

    if(serial->baud_rate == 0) {
        return astra_result_error(AstraStatusInvalidArgument, "serial baud rate is invalid");
    }

    return astra_result_ok();
}

AstraResult astra_device_serial_config_validate(const AstraDeviceSerialConfig* config) {
    if(!config) {
        return astra_result_error(AstraStatusInvalidArgument, "serial config is null");
    }

    if(config->channel >= AstraDeviceSerialChannelCount) {
        return astra_result_error(AstraStatusInvalidArgument, "serial channel is invalid");
    }

    if(config->baud_rate < ASTRA_DEVICE_SERIAL_MIN_BAUD_RATE ||
       config->baud_rate > ASTRA_DEVICE_SERIAL_MAX_BAUD_RATE) {
        return astra_result_error(AstraStatusInvalidArgument, "serial baud rate is invalid");
    }

    if(config->data_bits != 7u && config->data_bits != 8u) {
        return astra_result_error(AstraStatusInvalidArgument, "serial data bits is invalid");
    }

    if(config->parity >= AstraDeviceSerialParityCount) {
        return astra_result_error(AstraStatusInvalidArgument, "serial parity is invalid");
    }

    if(config->stop_bits >= AstraDeviceSerialStopBitsCount) {
        return astra_result_error(AstraStatusInvalidArgument, "serial stop bits is invalid");
    }

    if(config->flow_control >= AstraDeviceSerialFlowControlCount) {
        return astra_result_error(AstraStatusInvalidArgument, "serial flow control is invalid");
    }

    if(config->rx_buffer_size < ASTRA_DEVICE_SERIAL_MIN_BUFFER_SIZE ||
       config->rx_buffer_size > ASTRA_DEVICE_SERIAL_MAX_BUFFER_SIZE ||
       config->tx_buffer_size < ASTRA_DEVICE_SERIAL_MIN_BUFFER_SIZE ||
       config->tx_buffer_size > ASTRA_DEVICE_SERIAL_MAX_BUFFER_SIZE) {
        return astra_result_error(AstraStatusInvalidArgument, "serial buffer size is invalid");
    }

    return astra_result_ok();
}

AstraResult astra_device_serial_status_validate(const AstraDeviceSerialStatus* status) {
    if(!status) {
        return astra_result_error(AstraStatusInvalidArgument, "serial status is null");
    }

    if(!status->open && status->busy) {
        return astra_result_error(AstraStatusInvalidArgument, "serial status is invalid");
    }

    if(status->last_status > AstraStatusInternalError) {
        return astra_result_error(AstraStatusInvalidArgument, "serial status code is invalid");
    }

    return astra_result_ok();
}

AstraResult astra_device_serial_diagnostics_validate(
    const AstraDeviceSerialDiagnostics* diagnostics) {
    if(!diagnostics) {
        return astra_result_error(AstraStatusInvalidArgument, "serial diagnostics is null");
    }

    return astra_result_ok();
}

AstraResult astra_device_serial_session_validate(const AstraDeviceSerialSession* session) {
    if(!session) {
        return astra_result_error(AstraStatusInvalidArgument, "serial session is null");
    }

    if(session->active && session->session_id == 0) {
        return astra_result_error(AstraStatusInvalidArgument, "serial session id is invalid");
    }

    AstraResult result = astra_device_serial_config_validate(&session->config);
    if(result.status != AstraStatusOk) {
        return result;
    }

    result = astra_device_serial_status_validate(&session->status);
    if(result.status != AstraStatusOk) {
        return result;
    }

    return astra_device_serial_diagnostics_validate(&session->diagnostics);
}
