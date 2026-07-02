#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t baud_rate;
} AstraDeviceSerial;

#define ASTRA_DEVICE_SERIAL_MIN_BAUD_RATE 9600u
#define ASTRA_DEVICE_SERIAL_MAX_BAUD_RATE 4000000u
#define ASTRA_DEVICE_SERIAL_MIN_BUFFER_SIZE 1u
#define ASTRA_DEVICE_SERIAL_MAX_BUFFER_SIZE 4096u

typedef enum {
    AstraDeviceSerialChannelPrimary = 0,
    AstraDeviceSerialChannelSecondary,
    AstraDeviceSerialChannelCount,
} AstraDeviceSerialChannel;

typedef enum {
    AstraDeviceSerialParityNone = 0,
    AstraDeviceSerialParityEven,
    AstraDeviceSerialParityOdd,
    AstraDeviceSerialParityCount,
} AstraDeviceSerialParity;

typedef enum {
    AstraDeviceSerialStopBits1 = 0,
    AstraDeviceSerialStopBits2,
    AstraDeviceSerialStopBitsCount,
} AstraDeviceSerialStopBits;

typedef enum {
    AstraDeviceSerialFlowControlNone = 0,
    AstraDeviceSerialFlowControlRtsCts,
    AstraDeviceSerialFlowControlCount,
} AstraDeviceSerialFlowControl;

typedef struct {
    AstraDeviceSerialChannel channel;
    uint32_t baud_rate;
    uint8_t data_bits;
    AstraDeviceSerialParity parity;
    AstraDeviceSerialStopBits stop_bits;
    AstraDeviceSerialFlowControl flow_control;
    size_t rx_buffer_size;
    size_t tx_buffer_size;
    uint32_t timeout_ms;
} AstraDeviceSerialConfig;

typedef struct {
    bool open;
    bool busy;
    size_t rx_available;
    size_t tx_pending;
    AstraStatus last_status;
} AstraDeviceSerialStatus;

typedef struct {
    uint32_t rx_bytes;
    uint32_t tx_bytes;
    uint32_t frame_errors;
    uint32_t noise_errors;
    uint32_t overrun_errors;
    uint32_t timeout_count;
} AstraDeviceSerialDiagnostics;

typedef struct {
    uint32_t session_id;
    uint32_t sequence;
    bool active;
    AstraDeviceSerialConfig config;
    AstraDeviceSerialStatus status;
    AstraDeviceSerialDiagnostics diagnostics;
} AstraDeviceSerialSession;

AstraResult astra_device_serial_validate(const AstraDeviceSerial* serial);

AstraResult astra_device_serial_config_validate(const AstraDeviceSerialConfig* config);

AstraResult astra_device_serial_status_validate(const AstraDeviceSerialStatus* status);

AstraResult astra_device_serial_diagnostics_validate(
    const AstraDeviceSerialDiagnostics* diagnostics);

AstraResult astra_device_serial_session_validate(const AstraDeviceSerialSession* session);

#ifdef __cplusplus
}
#endif
