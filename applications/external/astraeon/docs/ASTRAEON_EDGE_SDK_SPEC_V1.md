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

Core:
- astra_event
- astra_context
- astra_errors

Communication:
- astra_uart
- astra_protocol
- astra_packet
- astra_sync

Storage:
- astra_storage
- astra_logger
- astra_config

UI:
- astra_console
- astra_menu
- astra_notification

Security:
- astra_policy
- astra_audit
- astra_validation

Integration:
- astra_node01
- astra_mqtt
- astra_homeassistant

## First Implementation Order

1. astra_event
2. astra_logger
3. astra_storage
4. astra_protocol
5. astra_uart
6. astra_console
7. astraeon_companion app

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

Initial transport is UART/USB Serial with JSONL.

## Safety

No replay of unknown access signals.
No unauthorized BadUSB.
No destructive payloads.
No public-area active testing.
