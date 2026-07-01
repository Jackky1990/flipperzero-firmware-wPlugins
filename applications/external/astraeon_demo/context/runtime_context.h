#pragma once

#include "astra_status.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    AstraeonGPIOWriteStateIdle = 0,
    AstraeonGPIOWriteStateConfirmRequired,
    AstraeonGPIOWriteStateActive,
    AstraeonGPIOWriteStateCompleted,
} AstraeonGPIOWriteState;

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
    bool supports_stream;
    bool supports_packets;
    bool reliable;
    uint32_t diagnostic_runs;
    uint32_t gpio_read_runs;
    uint32_t gpio_write_runs;
    uint32_t max_payload_size;
    uint16_t protocol_version;
    uint8_t gpio_adapter_pin_count;
    uint8_t gpio_read_pin;
    uint8_t gpio_read_status;
    uint8_t gpio_write_pin;
    uint8_t gpio_write_status;
    uint8_t gpio_write_state;
    uint8_t gpio_write_restore_count;
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
