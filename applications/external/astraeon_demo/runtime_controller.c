#include "runtime_controller.h"

#include "astra_event_builder.h"
#include "astra_flipper_gpio_adapter.h"
#include "astra_event_persistence.h"
#include "astra_logger.h"
#include "astra_node01.h"
#include "astra_node01_usb_bridge.h"
#include "astra_policy.h"
#include "astra_runtime_capabilities.h"
#include "astra_runtime_handshake.h"
#include "astra_runtime_heartbeat.h"
#include "astra_runtime_loopback.h"
#include "astra_runtime_ping.h"
#include "astra_runtime_transport.h"
#include "astra_storage.h"
#include "storage_adapter.h"

#include <stdio.h>
#include <string.h>

static void astraeon_demo_runtime_capture_transport(
    AstraeonRuntimeContext* runtime,
    const AstraRuntimeTransport* transport) {
    const AstraRuntimeTransportCapabilities* transport_capabilities =
        astra_runtime_transport_capabilities(transport);

    if(!transport_capabilities) {
        return;
    }

    runtime->supports_stream = transport_capabilities->supports_stream;
    runtime->supports_packets = transport_capabilities->supports_packets;
    runtime->reliable = transport_capabilities->reliable;
    runtime->max_payload_size = transport_capabilities->max_payload_size;
}

static bool astraeon_demo_runtime_check_capabilities(AstraeonRuntimeContext* runtime) {
    AstraRuntimeCapabilities local_capabilities;
    AstraRuntimeCapabilities remote_capabilities;
    AstraRuntimeCapabilities agreed_capabilities;

    if(astra_runtime_capabilities_init(&local_capabilities).status != AstraStatusOk ||
       astra_runtime_capabilities_init(&remote_capabilities).status != AstraStatusOk) {
        return false;
    }

    local_capabilities.supports_stream = runtime->supports_stream;
    local_capabilities.supports_packets = runtime->supports_packets;
    local_capabilities.reliable = runtime->reliable;
    local_capabilities.max_payload_size = runtime->max_payload_size;

    remote_capabilities = local_capabilities;

    return astra_runtime_handshake_negotiate(
               &local_capabilities,
               &remote_capabilities,
               &agreed_capabilities)
               .status == AstraStatusOk;
}

static bool astraeon_demo_runtime_check_handshake(AstraeonRuntimeContext* runtime) {
    AstraRuntimeHandshake handshake;

    if(astra_runtime_handshake_init(&handshake).status != AstraStatusOk) {
        return false;
    }

    handshake.max_payload_size = runtime->max_payload_size;
    runtime->protocol_version = handshake.protocol_version;

    return astra_runtime_handshake_validate(&handshake).status == AstraStatusOk;
}

static bool astraeon_demo_runtime_check_heartbeat(void) {
    AstraRuntimeHeartbeat heartbeat;

    return astra_runtime_heartbeat_init(&heartbeat, 1000, 3000).status == AstraStatusOk &&
           !astra_runtime_heartbeat_has_timed_out(&heartbeat, 1000) &&
           astra_runtime_heartbeat_should_send(&heartbeat, 1000);
}

static bool astraeon_demo_runtime_check_policy(void) {
    AstraPolicy policy;
    uint8_t payload = 0;

    return astra_policy_init(&policy).status == AstraStatusOk &&
           astra_policy_validate_command(&policy, 1, &payload, sizeof(payload)).status ==
               AstraStatusOk;
}

static bool astraeon_demo_runtime_check_storage(
    AstraStorage* storage,
    AstraeonStorageAdapter* adapter,
    Storage* furi_storage) {
    return astra_storage_init(storage).status == AstraStatusOk &&
           astraeon_storage_adapter_init(adapter, furi_storage).status == AstraStatusOk &&
           astraeon_storage_adapter_bind(storage, adapter).status == AstraStatusOk &&
           astra_storage_validate_path(ASTRA_STORAGE_LOG_PATH).status == AstraStatusOk;
}

static bool astraeon_demo_runtime_check_logger(
    AstraStorage* storage,
    AstraeonStorageAdapter* adapter) {
    AstraLogger logger;

    return astra_logger_init(&logger, storage).status == AstraStatusOk &&
           astra_logger_log(&logger, AstraLogLevelInfo, "diagnostics").status == AstraStatusOk &&
           adapter->append_calls == 1 &&
           adapter->append_bytes > 0;
}

static bool astraeon_demo_runtime_check_event_persistence(
    AstraStorage* storage,
    uint32_t diagnostic_runs) {
    AstraEvent event;
    AstraEventPersistence persistence;

    if(astra_event_builder_init(&event, AstraEventTypeDiagnosticsReport).status != AstraStatusOk ||
       astra_event_persistence_init(&persistence, storage).status != AstraStatusOk) {
        return false;
    }

    snprintf(event.id, sizeof(event.id), "EVT-FLP-DIAG-%lu", (unsigned long)diagnostic_runs);
    snprintf(event.source_device, sizeof(event.source_device), "FLP-JACK-01");
    snprintf(event.target_device, sizeof(event.target_device), "NODE01");

    event.category = AstraEventCategoryDiagnostics;
    event.state = AstraEventStateCompleted;
    event.priority = AstraEventPriorityNormal;
    event.flags = AstraEventFlagPersist;
    event.timestamp = diagnostic_runs;
    event.source_module = AstraModuleDiagnostics;
    event.status = AstraStatusOk;

    return astra_event_persistence_append(&persistence, &event).status == AstraStatusOk;
}

static bool astraeon_demo_runtime_check_node01(
    AstraRuntimeTransport* transport,
    uint32_t diagnostic_runs) {
    AstraEvent event;
    AstraNode01 node;

    if(astra_event_builder_init(&event, AstraEventTypeDiagnosticsReport).status != AstraStatusOk ||
       astra_node01_init(&node, transport).status != AstraStatusOk) {
        return false;
    }

    snprintf(event.id, sizeof(event.id), "EVT-FLP-NODE01-%lu", (unsigned long)diagnostic_runs);
    event.category = AstraEventCategoryDiagnostics;
    event.state = AstraEventStateCompleted;
    event.priority = AstraEventPriorityNormal;
    event.flags = AstraEventFlagPersist;
    event.timestamp = diagnostic_runs;
    event.source_module = AstraModuleDiagnostics;
    event.status = AstraStatusOk;

    return astra_node01_send_event(&node, &event).status == AstraStatusOk;
}

static bool astraeon_demo_runtime_check_node01_usb(uint32_t diagnostic_runs) {
    AstraEvent event;
    AstraNode01UsbBridge bridge;

    if(astra_event_builder_init(&event, AstraEventTypeDiagnosticsReport).status != AstraStatusOk ||
       astra_node01_usb_bridge_init(&bridge).status != AstraStatusOk) {
        return false;
    }

    snprintf(event.id, sizeof(event.id), "EVT-FLP-USB-%lu", (unsigned long)diagnostic_runs);
    event.timestamp = diagnostic_runs;
    event.source_module = AstraModuleDiagnostics;
    event.status = AstraStatusOk;

    return astra_node01_usb_bridge_send_event(&bridge, &event).status == AstraStatusOk;
}

static bool astraeon_demo_runtime_check_gpio_adapter(AstraeonRuntimeContext* runtime) {
    AstraFlipperGPIOAdapter adapter;

    if(astra_flipper_gpio_adapter_init(&adapter).status != AstraStatusOk ||
       astra_flipper_gpio_adapter_bind_resources(&adapter).status != AstraStatusOk ||
       astra_flipper_gpio_adapter_validate(&adapter).status != AstraStatusOk) {
        return false;
    }

    size_t pin_count = astra_flipper_gpio_adapter_pin_count(&adapter);
    if(pin_count != AstraFlipperGPIOPinCount) {
        return false;
    }

    for(size_t index = 0; index < pin_count; ++index) {
        const AstraFlipperGPIOPinBinding* binding =
            astra_flipper_gpio_adapter_pin_at(&adapter, index);
        if(!binding || !binding->flipper_pin) {
            return false;
        }
    }

    runtime->gpio_adapter_pin_count = (uint8_t)pin_count;
    runtime->gpio_adapter_bound = true;
    return true;
}

static void astraeon_demo_runtime_record_gpio_session(
    AstraStorage* storage,
    const char* session_id,
    uint32_t validation_runs,
    AstraStatus status) {
    AstraLogger logger;
    AstraEvent event;
    AstraEventPersistence persistence;

    if(astra_logger_init(&logger, storage).status == AstraStatusOk) {
        char line[96];
        snprintf(
            line,
            sizeof(line),
            "gpio session=%s end status=%s",
            session_id,
            astra_status_to_string(status));
        astra_logger_log(&logger, AstraLogLevelInfo, line);
    }

    if(astra_event_builder_init(&event, AstraEventTypeDiagnosticsReport).status != AstraStatusOk ||
       astra_event_persistence_init(&persistence, storage).status != AstraStatusOk) {
        return;
    }

    snprintf(event.id, sizeof(event.id), "%s", session_id);
    snprintf(event.source_device, sizeof(event.source_device), "FLP-JACK-01");
    snprintf(event.target_device, sizeof(event.target_device), "NODE01");

    event.category = AstraEventCategoryDiagnostics;
    event.state = AstraEventStateCompleted;
    event.priority = AstraEventPriorityNormal;
    event.flags = AstraEventFlagPersist;
    event.timestamp = validation_runs;
    event.source_module = AstraModuleDiagnostics;
    event.status = status;

    astra_event_persistence_append(&persistence, &event);
}

static void astraeon_demo_runtime_log_gpio_step(
    AstraStorage* storage,
    const char* session_id,
    const char* step,
    AstraStatus status) {
    AstraLogger logger;
    char line[96];

    if(astra_logger_init(&logger, storage).status != AstraStatusOk) {
        return;
    }

    snprintf(
        line,
        sizeof(line),
        "gpio session=%s step=%s status=%s",
        session_id,
        step,
        astra_status_to_string(status));
    astra_logger_log(&logger, AstraLogLevelInfo, line);
}

AstraStatus astraeon_demo_runtime_controller_cancel_gpio_write(AstraeonRuntimeContext* runtime) {
    return astraeon_runtime_gpio_write_cancel(runtime);
}

AstraStatus astraeon_demo_runtime_controller_timeout_gpio_write(AstraeonRuntimeContext* runtime) {
    return astraeon_runtime_gpio_write_timeout(runtime);
}

AstraStatus astraeon_demo_runtime_controller_request_gpio_write(
    AstraeonRuntimeContext* runtime,
    Storage* furi_storage) {
    if(!runtime) {
        return AstraStatusInvalidArgument;
    }

    if(!runtime->gpio_write_confirm_required) {
        return astraeon_runtime_gpio_write_request_confirmation(runtime);
    }

    AstraStatus begin_status = astraeon_runtime_gpio_write_begin(runtime);
    if(begin_status != AstraStatusOk) {
        return begin_status;
    }

    bool binding_ok = false;
    bool output_mode_changed = false;
    bool low_final_attempted = false;
    AstraResult session_result;
    AstraResult step_result;
    AstraResult restore_result = astra_result_ok();
    AstraFlipperGPIOAdapter adapter;
    AstraFlipperGPIOPinId pin = AstraFlipperGPIOPinPC0;
    uint32_t validation_runs = runtime->gpio_write_runs;
    char session_id[32];

    snprintf(
        session_id,
        sizeof(session_id),
        "EVT-FLP-GPIO-WRITE-%lu",
        (unsigned long)validation_runs);

    runtime->gpio_write_pin = (uint8_t)pin;
    runtime->gpio_write_status = AstraStatusInternalError;

    session_result = astra_flipper_gpio_adapter_init(&adapter);
    if(session_result.status == AstraStatusOk) {
        session_result = astra_flipper_gpio_adapter_bind_resources(&adapter);
        binding_ok = session_result.status == AstraStatusOk;
    }

    AstraStorage astra_storage;
    AstraeonStorageAdapter storage_adapter;
    bool storage_ready =
        astraeon_demo_runtime_check_storage(&astra_storage, &storage_adapter, furi_storage);

    if(storage_ready) {
        astraeon_demo_runtime_log_gpio_step(
            &astra_storage,
            session_id,
            "begin",
            session_result.status);
    }

    if(session_result.status == AstraStatusOk) {
        session_result = astra_flipper_gpio_adapter_set_output_low_mode(
            &adapter,
            pin,
            &output_mode_changed);
        astraeon_runtime_gpio_write_set_mode_changed(runtime, output_mode_changed);
        if(storage_ready) {
            astraeon_demo_runtime_log_gpio_step(
                &astra_storage,
                session_id,
                "output-low",
                session_result.status);
        }
    }

    if(session_result.status == AstraStatusOk) {
        session_result = astra_flipper_gpio_adapter_write_pin(&adapter, pin, true);
        if(storage_ready) {
            astraeon_demo_runtime_log_gpio_step(
                &astra_storage,
                session_id,
                "output-high",
                session_result.status);
        }
    }

    if(session_result.status == AstraStatusOk) {
        session_result = astra_flipper_gpio_adapter_write_pin(&adapter, pin, false);
        low_final_attempted = true;
        if(storage_ready) {
            astraeon_demo_runtime_log_gpio_step(
                &astra_storage,
                session_id,
                "output-low-final",
                session_result.status);
        }
    }

    if(output_mode_changed) {
        if(!low_final_attempted) {
            step_result = astra_flipper_gpio_adapter_write_pin(&adapter, pin, false);
            if(storage_ready) {
                astraeon_demo_runtime_log_gpio_step(
                    &astra_storage,
                    session_id,
                    "output-low-final",
                    step_result.status);
            }
            if(session_result.status == AstraStatusOk && step_result.status != AstraStatusOk) {
                session_result = step_result;
            }
        }

        restore_result = astra_flipper_gpio_adapter_restore_if_needed(&adapter, pin);
        astraeon_runtime_gpio_write_mark_restore(runtime);
        if(storage_ready) {
            astraeon_demo_runtime_log_gpio_step(
                &astra_storage,
                session_id,
                "restore",
                restore_result.status);
        }
        if(session_result.status == AstraStatusOk && restore_result.status != AstraStatusOk) {
            session_result = restore_result;
        }
    }

    runtime->gpio_adapter_bound = binding_ok;
    astraeon_runtime_gpio_write_finish(runtime, session_result.status);

    if(storage_ready) {
        astraeon_demo_runtime_record_gpio_session(
            &astra_storage,
            session_id,
            validation_runs,
            session_result.status);
        astraeon_demo_runtime_log_gpio_step(
            &astra_storage,
            session_id,
            "end",
            session_result.status);
    }

    return session_result.status;
}

void astraeon_demo_runtime_controller_validate_gpio_read(
    AstraeonRuntimeContext* runtime,
    Storage* furi_storage) {
    if(!runtime) {
        return;
    }

    bool value = false;
    bool binding_ok = false;
    bool input_mode_changed = false;
    AstraResult read_result;
    AstraResult setup_result;
    AstraResult restore_result = astra_result_ok();
    AstraFlipperGPIOAdapter adapter;
    AstraFlipperGPIOPinId pin = AstraFlipperGPIOPinPC0;
    uint32_t validation_runs = runtime->gpio_read_runs + 1;
    char session_id[32];

    snprintf(
        session_id,
        sizeof(session_id),
        "EVT-FLP-GPIO-READ-%lu",
        (unsigned long)validation_runs);

    runtime->gpio_read_runs = validation_runs;
    runtime->gpio_read_checked = true;
    runtime->gpio_read_ok = false;
    runtime->gpio_read_value = false;
    runtime->gpio_read_pin = (uint8_t)pin;
    runtime->gpio_read_status = AstraStatusInternalError;

    read_result = astra_flipper_gpio_adapter_init(&adapter);
    if(read_result.status == AstraStatusOk) {
        read_result = astra_flipper_gpio_adapter_bind_resources(&adapter);
        binding_ok = read_result.status == AstraStatusOk;
    }

    AstraStorage astra_storage;
    AstraeonStorageAdapter storage_adapter;
    bool storage_ready =
        astraeon_demo_runtime_check_storage(&astra_storage, &storage_adapter, furi_storage);

    if(storage_ready) {
        astraeon_demo_runtime_log_gpio_step(
            &astra_storage,
            session_id,
            "begin",
            read_result.status);
    }

    setup_result = read_result;
    if(read_result.status == AstraStatusOk) {
        setup_result = astra_flipper_gpio_adapter_set_input_mode(
            &adapter,
            pin,
            &input_mode_changed);
        if(storage_ready) {
            astraeon_demo_runtime_log_gpio_step(
                &astra_storage,
                session_id,
                input_mode_changed ? "setup-changed" : "setup-ready",
                setup_result.status);
        }
    }

    read_result = setup_result;
    if(setup_result.status == AstraStatusOk) {
        read_result = astra_flipper_gpio_adapter_read_pin(&adapter, pin, &value);
        if(storage_ready) {
            astraeon_demo_runtime_log_gpio_step(
                &astra_storage,
                session_id,
                "read",
                read_result.status);
        }
    }

    if(input_mode_changed) {
        restore_result = astra_flipper_gpio_adapter_restore_if_needed(&adapter, pin);
        if(storage_ready) {
            astraeon_demo_runtime_log_gpio_step(
                &astra_storage,
                session_id,
                "restore",
                restore_result.status);
        }
        if(read_result.status == AstraStatusOk && restore_result.status != AstraStatusOk) {
            read_result = restore_result;
        }
    }

    runtime->gpio_adapter_bound = binding_ok;
    runtime->gpio_read_status = read_result.status;
    runtime->gpio_read_ok = read_result.status == AstraStatusOk;
    runtime->gpio_read_value = runtime->gpio_read_ok && value;

    if(storage_ready) {
        astraeon_demo_runtime_record_gpio_session(
            &astra_storage,
            session_id,
            validation_runs,
            read_result.status);
    }
}

void astraeon_demo_runtime_controller_start(
    AstraeonRuntimeContext* runtime,
    Storage* furi_storage) {
    if(!runtime) {
        return;
    }

    uint32_t diagnostic_runs = runtime->diagnostic_runs + 1;
    memset(runtime, 0, sizeof(*runtime));
    runtime->diagnostic_runs = diagnostic_runs;

    AstraRuntimeTransport transport;

    runtime->ready = true;
    runtime->transport_ready =
        astra_runtime_transport_init(&transport).status == AstraStatusOk &&
        astra_runtime_loopback_init(&transport).status == AstraStatusOk;

    if(runtime->transport_ready) {
        astraeon_demo_runtime_capture_transport(runtime, &transport);
    }

    runtime->capabilities_ok = astraeon_demo_runtime_check_capabilities(runtime);
    runtime->handshake_ok =
        runtime->capabilities_ok && astraeon_demo_runtime_check_handshake(runtime);
    runtime->heartbeat_ok = astraeon_demo_runtime_check_heartbeat();
    runtime->policy_ok = astraeon_demo_runtime_check_policy();

    AstraStorage astra_storage;
    AstraeonStorageAdapter storage_adapter;

    runtime->storage_ok =
        astraeon_demo_runtime_check_storage(&astra_storage, &storage_adapter, furi_storage);
    runtime->logger_ok =
        runtime->storage_ok && astraeon_demo_runtime_check_logger(&astra_storage, &storage_adapter);
    runtime->event_persistence_ok =
        runtime->storage_ok &&
        astraeon_demo_runtime_check_event_persistence(&astra_storage, diagnostic_runs);
    runtime->node01_ok =
        runtime->transport_ready && astraeon_demo_runtime_check_node01(&transport, diagnostic_runs);
    runtime->node01_usb_ok = astraeon_demo_runtime_check_node01_usb(diagnostic_runs);
    runtime->gpio_adapter_ok = astraeon_demo_runtime_check_gpio_adapter(runtime);

    runtime->ping_ok =
        runtime->transport_ready &&
        astra_runtime_ping_send(&transport).status == AstraStatusOk &&
        astra_runtime_ping_receive(&transport).status == AstraStatusOk;
}
