#pragma once

#include "astra_device_serial.h"
#include "astra_status.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    AstraeonGPIOWriteStateIdle = 0,
    AstraeonGPIOWriteStateConfirmRequired,
    AstraeonGPIOWriteStateActive,
    AstraeonGPIOWriteStateCompleted,
} AstraeonGPIOWriteState;

typedef enum {
    AstraeonUARTSessionStateIdle = 0,
    AstraeonUARTSessionStateOpenRequested,
    AstraeonUARTSessionStateAcquired,
    AstraeonUARTSessionStateConfigured,
    AstraeonUARTSessionStateActive,
    AstraeonUARTSessionStateClosing,
    AstraeonUARTSessionStateClosed,
    AstraeonUARTSessionStateError,
} AstraeonUARTSessionState;

typedef struct {
    bool ready;
    bool transport_ready;
    bool ping_ok;
    bool capabilities_ok;
    bool handshake_ok;
    bool heartbeat_ok;
    bool policy_ok;
    bool storage_ok;
    bool logger_ok;
    bool event_persistence_ok;
    bool node01_ok;
    bool node01_usb_ok;
    bool gpio_adapter_ok;
    bool gpio_adapter_bound;
    bool gpio_read_checked;
    bool gpio_read_ok;
    bool gpio_read_value;
    bool gpio_write_checked;
    bool gpio_write_ok;
    bool gpio_write_confirm_required;
    bool gpio_write_session_active;
    bool gpio_write_mode_changed;
    bool gpio_write_restored;
    bool uart_checked;
    bool uart_ok;
    bool uart_session_active;
    bool uart_acquired;
    bool uart_configured;
    bool uart_released;
    bool uart_tx_checked;
    bool uart_tx_ok;
    bool uart_tx_active;
    bool supports_stream;
    bool supports_packets;
    bool reliable;
    uint32_t diagnostic_runs;
    uint32_t gpio_read_runs;
    uint32_t gpio_write_runs;
    uint32_t uart_runs;
    uint32_t uart_session_id;
    uint32_t uart_baud_rate;
    uint32_t uart_tx_runs;
    uint32_t uart_tx_bytes_requested;
    uint32_t uart_tx_bytes_written;
    uint32_t max_payload_size;
    uint16_t protocol_version;
    uint8_t gpio_adapter_pin_count;
    uint8_t gpio_read_pin;
    uint8_t gpio_read_status;
    uint8_t gpio_write_pin;
    uint8_t gpio_write_status;
    uint8_t gpio_write_state;
    uint8_t gpio_write_restore_count;
    uint8_t uart_channel;
    uint8_t uart_status;
    uint8_t uart_state;
    uint8_t uart_release_count;
    uint8_t uart_tx_status;
} AstraeonRuntimeContext;

static inline AstraStatus
    astraeon_runtime_gpio_write_request_confirmation(AstraeonRuntimeContext* runtime) {
    if(!runtime) {
        return AstraStatusInvalidArgument;
    }

    if(runtime->gpio_write_session_active) {
        runtime->gpio_write_checked = true;
        runtime->gpio_write_ok = false;
        runtime->gpio_write_status = AstraStatusBusy;
        return AstraStatusBusy;
    }

    runtime->gpio_write_confirm_required = true;
    runtime->gpio_write_state = AstraeonGPIOWriteStateConfirmRequired;
    runtime->gpio_write_status = AstraStatusOk;
    return AstraStatusOk;
}

static inline AstraStatus astraeon_runtime_gpio_write_begin(AstraeonRuntimeContext* runtime) {
    if(!runtime) {
        return AstraStatusInvalidArgument;
    }

    if(runtime->gpio_write_session_active) {
        runtime->gpio_write_checked = true;
        runtime->gpio_write_ok = false;
        runtime->gpio_write_status = AstraStatusBusy;
        return AstraStatusBusy;
    }

    if(!runtime->gpio_write_confirm_required) {
        runtime->gpio_write_checked = true;
        runtime->gpio_write_ok = false;
        runtime->gpio_write_status = AstraStatusPermissionDenied;
        return AstraStatusPermissionDenied;
    }

    runtime->gpio_write_runs += 1;
    runtime->gpio_write_checked = true;
    runtime->gpio_write_ok = false;
    runtime->gpio_write_confirm_required = false;
    runtime->gpio_write_session_active = true;
    runtime->gpio_write_mode_changed = false;
    runtime->gpio_write_restored = false;
    runtime->gpio_write_restore_count = 0;
    runtime->gpio_write_state = AstraeonGPIOWriteStateActive;
    runtime->gpio_write_status = AstraStatusInternalError;
    return AstraStatusOk;
}

static inline AstraStatus astraeon_runtime_gpio_write_cancel(AstraeonRuntimeContext* runtime) {
    if(!runtime) {
        return AstraStatusInvalidArgument;
    }

    runtime->gpio_write_checked = true;
    runtime->gpio_write_ok = false;
    runtime->gpio_write_confirm_required = false;
    runtime->gpio_write_session_active = false;
    runtime->gpio_write_state = AstraeonGPIOWriteStateCompleted;
    runtime->gpio_write_status = AstraStatusPolicyDenied;
    return AstraStatusPolicyDenied;
}

static inline AstraStatus astraeon_runtime_gpio_write_timeout(AstraeonRuntimeContext* runtime) {
    if(!runtime) {
        return AstraStatusInvalidArgument;
    }

    runtime->gpio_write_checked = true;
    runtime->gpio_write_ok = false;
    runtime->gpio_write_confirm_required = false;
    runtime->gpio_write_session_active = false;
    runtime->gpio_write_state = AstraeonGPIOWriteStateCompleted;
    runtime->gpio_write_status = AstraStatusTimeout;
    return AstraStatusTimeout;
}

static inline void astraeon_runtime_gpio_write_set_mode_changed(
    AstraeonRuntimeContext* runtime,
    bool changed) {
    if(runtime) {
        runtime->gpio_write_mode_changed = changed;
    }
}

static inline void astraeon_runtime_gpio_write_mark_restore(AstraeonRuntimeContext* runtime) {
    if(runtime && runtime->gpio_write_mode_changed && !runtime->gpio_write_restored) {
        runtime->gpio_write_restored = true;
        runtime->gpio_write_restore_count += 1;
    }
}

static inline AstraStatus
    astraeon_runtime_gpio_write_finish(AstraeonRuntimeContext* runtime, AstraStatus status) {
    if(!runtime) {
        return AstraStatusInvalidArgument;
    }

    runtime->gpio_write_checked = true;
    runtime->gpio_write_ok = status == AstraStatusOk;
    runtime->gpio_write_session_active = false;
    runtime->gpio_write_confirm_required = false;
    runtime->gpio_write_state = AstraeonGPIOWriteStateCompleted;
    runtime->gpio_write_status = status;
    return status;
}

static inline AstraStatus astraeon_runtime_uart_open_begin(
    AstraeonRuntimeContext* runtime,
    const AstraDeviceSerialConfig* config) {
    if(!runtime) {
        return AstraStatusInvalidArgument;
    }

    if(runtime->uart_session_active) {
        runtime->uart_checked = true;
        runtime->uart_ok = false;
        runtime->uart_status = AstraStatusBusy;
        return AstraStatusBusy;
    }

    if(astra_device_serial_config_validate(config).status != AstraStatusOk) {
        runtime->uart_checked = true;
        runtime->uart_ok = false;
        runtime->uart_session_active = false;
        runtime->uart_acquired = false;
        runtime->uart_configured = false;
        runtime->uart_released = false;
        runtime->uart_state = AstraeonUARTSessionStateError;
        runtime->uart_status = AstraStatusInvalidArgument;
        return AstraStatusInvalidArgument;
    }

    runtime->uart_runs += 1;
    runtime->uart_session_id = runtime->uart_runs;
    runtime->uart_checked = true;
    runtime->uart_ok = false;
    runtime->uart_session_active = true;
    runtime->uart_acquired = false;
    runtime->uart_configured = false;
    runtime->uart_released = false;
    runtime->uart_channel = (uint8_t)config->channel;
    runtime->uart_baud_rate = config->baud_rate;
    runtime->uart_release_count = 0;
    runtime->uart_state = AstraeonUARTSessionStateOpenRequested;
    runtime->uart_status = AstraStatusInternalError;
    return AstraStatusOk;
}

static inline AstraStatus astraeon_runtime_uart_mark_acquired(AstraeonRuntimeContext* runtime) {
    if(!runtime) {
        return AstraStatusInvalidArgument;
    }

    if(!runtime->uart_session_active) {
        runtime->uart_checked = true;
        runtime->uart_ok = false;
        runtime->uart_status = AstraStatusInvalidArgument;
        return AstraStatusInvalidArgument;
    }

    runtime->uart_acquired = true;
    runtime->uart_state = AstraeonUARTSessionStateAcquired;
    return AstraStatusOk;
}

static inline AstraStatus astraeon_runtime_uart_mark_configured(AstraeonRuntimeContext* runtime) {
    if(!runtime) {
        return AstraStatusInvalidArgument;
    }

    if(!runtime->uart_session_active || !runtime->uart_acquired) {
        runtime->uart_checked = true;
        runtime->uart_ok = false;
        runtime->uart_status = AstraStatusInvalidArgument;
        return AstraStatusInvalidArgument;
    }

    runtime->uart_configured = true;
    runtime->uart_state = AstraeonUARTSessionStateConfigured;
    return AstraStatusOk;
}

static inline AstraStatus astraeon_runtime_uart_mark_active(AstraeonRuntimeContext* runtime) {
    if(!runtime) {
        return AstraStatusInvalidArgument;
    }

    if(!runtime->uart_session_active || !runtime->uart_acquired || !runtime->uart_configured) {
        runtime->uart_checked = true;
        runtime->uart_ok = false;
        runtime->uart_status = AstraStatusInvalidArgument;
        return AstraStatusInvalidArgument;
    }

    runtime->uart_state = AstraeonUARTSessionStateActive;
    runtime->uart_status = AstraStatusOk;
    return AstraStatusOk;
}

static inline AstraStatus astraeon_runtime_uart_close_begin(AstraeonRuntimeContext* runtime) {
    if(!runtime) {
        return AstraStatusInvalidArgument;
    }

    if(!runtime->uart_session_active) {
        runtime->uart_checked = true;
        runtime->uart_ok = false;
        runtime->uart_status = AstraStatusInvalidArgument;
        return AstraStatusInvalidArgument;
    }

    runtime->uart_state = AstraeonUARTSessionStateClosing;
    return AstraStatusOk;
}

static inline void astraeon_runtime_uart_mark_release(AstraeonRuntimeContext* runtime) {
    if(runtime && runtime->uart_acquired && !runtime->uart_released) {
        runtime->uart_released = true;
        runtime->uart_release_count += 1;
    }
}

static inline AstraStatus
    astraeon_runtime_uart_finish(AstraeonRuntimeContext* runtime, AstraStatus status) {
    if(!runtime) {
        return AstraStatusInvalidArgument;
    }

    runtime->uart_checked = true;
    runtime->uart_ok = status == AstraStatusOk;
    runtime->uart_session_active = false;
    runtime->uart_state =
        status == AstraStatusOk ? AstraeonUARTSessionStateClosed : AstraeonUARTSessionStateError;
    runtime->uart_status = status;
    return status;
}

static inline AstraStatus astraeon_runtime_uart_cancel(AstraeonRuntimeContext* runtime) {
    if(!runtime) {
        return AstraStatusInvalidArgument;
    }

    astraeon_runtime_uart_mark_release(runtime);
    return astraeon_runtime_uart_finish(runtime, AstraStatusPolicyDenied);
}

static inline AstraStatus astraeon_runtime_uart_timeout(AstraeonRuntimeContext* runtime) {
    if(!runtime) {
        return AstraStatusInvalidArgument;
    }

    astraeon_runtime_uart_mark_release(runtime);
    return astraeon_runtime_uart_finish(runtime, AstraStatusTimeout);
}

static inline AstraStatus astraeon_runtime_uart_tx_begin(
    AstraeonRuntimeContext* runtime,
    const uint8_t* data,
    uint32_t length) {
    if(!runtime) {
        return AstraStatusInvalidArgument;
    }

    runtime->uart_tx_checked = true;
    runtime->uart_tx_ok = false;

    if(!runtime->uart_session_active) {
        runtime->uart_tx_active = false;
        runtime->uart_tx_status = AstraStatusPermissionDenied;
        return AstraStatusPermissionDenied;
    }

    if(runtime->uart_session_id == 0 ||
       runtime->uart_state != AstraeonUARTSessionStateActive ||
       !runtime->uart_acquired ||
       !runtime->uart_configured ||
       runtime->uart_released) {
        runtime->uart_tx_active = false;
        runtime->uart_tx_status = AstraStatusInvalidArgument;
        return AstraStatusInvalidArgument;
    }

    if(length > 0 && !data) {
        runtime->uart_tx_active = false;
        runtime->uart_tx_status = AstraStatusInvalidArgument;
        return AstraStatusInvalidArgument;
    }

    runtime->uart_tx_runs += 1;
    runtime->uart_tx_active = true;
    runtime->uart_tx_bytes_requested = length;
    runtime->uart_tx_bytes_written = 0;
    runtime->uart_tx_status = AstraStatusInternalError;
    return AstraStatusOk;
}

static inline AstraStatus astraeon_runtime_uart_tx_finish(
    AstraeonRuntimeContext* runtime,
    AstraStatus status,
    uint32_t written) {
    if(!runtime) {
        return AstraStatusInvalidArgument;
    }

    runtime->uart_tx_checked = true;
    runtime->uart_tx_ok = status == AstraStatusOk;
    runtime->uart_tx_active = false;
    runtime->uart_tx_bytes_written = written;
    runtime->uart_tx_status = status;
    return status;
}
