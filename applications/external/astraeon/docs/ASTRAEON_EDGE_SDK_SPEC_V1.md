# ASTRAEON Edge SDK Spec v1.0

## Purpose

Flipper Zero RogueMaster will act as an ASTRAEON-X Field Terminal.

It will not be the AI brain.
It will scan, log, control authorized devices, and sync events to Node01.

## Core Rules

- Do not modify RogueMaster core unless required.
- Prefer external apps under applications/external/astraeon/.
- All modules must be small and separated.
- No direct hardware calls from app UI.
- Use SDK wrappers for UART, Storage, Logger, Event, Protocol.
- Every command/event must pass policy validation.
- Unknown signals are log-only.
- Authorized devices only.

## Directory Layout

applications/external/astraeon/
├── docs/
├── sdk/
├── companion/
├── inventory/
├── mission/
├── diagnostics/
├── automation/
└── common/

## SDK Modules

Implemented foundation:
- astra_status
- astra_types
- astra_result
- astra_version
- astra_build_info

Implemented runtime:
- astra_runtime_context
- astra_runtime_default
- astra_runtime_manager
- astra_runtime_session
- astra_runtime_handshake
- astra_runtime_capabilities
- astra_runtime_heartbeat
- astra_runtime_message
- astra_runtime_codec
- astra_runtime_transport
- astra_runtime_loopback
- astra_runtime_usb_transport
- astra_runtime_ping
- astra_event
- astra_event_bus
- astra_event_queue
- astra_dispatcher
- astra_registry
- astra_handler
- astra_scheduler
- astra_event_source
- astra_event_builder
- astra_event_lifecycle
- astra_event_id
- astra_event_types

Implemented services:
- astra_policy
- astra_storage
- astra_logger
- astra_event_persistence
- astra_node01
- astra_node01_usb_bridge

Planned services:
- astra_audit
- astra_validation

Planned integration:
- astra_mqtt
- astra_homeassistant

## First Implementation Order

1. Keep `applications/external/astraeon/sdk` as the canonical SDK.
2. Keep `astraeon_demo` exercising only SDK APIs.
3. Stabilize runtime session, handshake, capabilities, heartbeat, message,
   codec, and transport behavior.
4. Add storage/logger/policy services after runtime verification is stable.
5. Add Node01 integration after command/event validation exists.

## Event Format

```json
{
  "id": "EVT-FLP-YYYYMMDD-HHMMSS-TYPE",
  "type": "status",
  "source": "FLP-JACK-01",
  "zone": "workshop",
  "payload": {}
}

```

## Storage Paths

/ext/astraeon/events/flipper_events.jsonl
/ext/astraeon/logs/astra.log
/ext/astraeon/config/device.json
/ext/astraeon/cache/

## Transport v0.1

Initial safe transport is loopback. USB CDC transport is available behind
`ASTRA_RUNTIME_ENABLE_USB_CDC`. JSONL over UART/USB remains the Node01 adapter
target and should be introduced after the runtime message contract is stable.

## Safety

No replay of unknown access signals.
No unauthorized BadUSB.
No destructive payloads.
No public-area active testing.
