# ASTRAEON Ecosystem Architecture

## System Overview

ASTRAEON is organized as layered systems that separate vision, knowledge,
validation, and execution.

```text
AI / Architect / Operator
        |
        v
Vision Layer + Knowledge Layer
        |
        v
Kernel + Core Services
        |
        v
Commander + Automation
        |
        v
Hardware Lab + Probe Network
        |
        v
Reference Targets and Robotics
```

## Core Domains

- Kernel
- Core
- AI
- Hardware
- Vision
- Knowledge
- Automation
- Network
- Security
- Robotics
- Labs
- SDK
- Plugins

## Kernel Architecture

The ASTRAEON Kernel is the common control plane for events, devices, resources,
permissions, scheduling, plugins, telemetry, diagnostics, and recovery.

Kernel services:

- Event Kernel
- Device Kernel
- Session Kernel
- Resource Kernel
- Permission Kernel
- Driver Kernel
- Scheduler
- Plugin Manager
- Logger
- Telemetry
- Diagnostics
- Recovery
- API Gateway

Kernel rules:

- Events are the source of truth.
- Sessions bind commands, evidence, and results.
- Resources must be acquired before use.
- Permissions are checked before control.
- Drivers stay thin and hardware-specific.
- Recovery must be explicit and observable.

## Hardware Lab

The ASTRAEON Hardware Lab validates real devices through external probe nodes
and reproducible validation recipes.

Hardware Lab components:

- ASTRAEON Probe Nodes
- Commander
- Probe Network
- Validation Engine
- Digital Twin
- Hardware Registry
- Telemetry
- Packet Logger
- Logic Capture

The lab must observe before control. It must never depend on manual-only
evidence when a probe can capture structured events.

## ASTRAEON Probe Architecture

ASTRAEON Probe is not just a UART bridge. It is reusable hardware debug
infrastructure.

Initial hardware:

- Flipper Wi-Fi Dev Board ESP32-S2

Probe modules:

- UART
- GPIO
- SPI
- I2C
- Logic Capture
- Packet Logger
- USB CDC
- WiFi Bridge
- OTA
- Diagnostics
- Self Test

Probe constraints:

- Separate project from RogueMaster.
- No RogueMaster build-system dependency.
- 3.3V logic by default.
- Passive monitoring before active drive.
- Pin allowlists per target.
- Versioned firmware and protocol.

## Commander Architecture

Commander coordinates probe sessions and validation runs.

Initial form:

- CLI

Later forms:

- Desktop app
- Web dashboard
- Node01 service integration

Commander responsibilities:

- Probe discovery
- Session creation
- Command dispatch
- Log/event capture
- Validation recipe execution
- PASS/FAIL report generation
- Automation hooks

## Protocol Design

Protocol goals:

- Human-readable first.
- JSONL/log compatible.
- Session-correlated.
- Future binary framing for high-rate capture.

Command format:

```json
{"type":"CMD","id":"cmd-001","session":"lab-001","target":"probe-01","op":"uart.listen","args":{"channel":"uart0","baud":115200}}
```

Event format:

```json
{"type":"EVENT","seq":1,"session":"lab-001","source":"probe-01.uart0","event":"rx.payload","status":"OK","bytes":22}
```

Packet framing:

- P0-P4: newline-delimited JSONL over USB CDC or TCP.
- P5 and later: binary payload frame with JSON metadata envelope.
- Every frame carries sequence number, timestamp, source, session, and status.

## Vision Layer

The Vision Layer describes the current and historical state of the hardware
ecosystem.

Vision entities:

- Topology
- Connections
- Signals
- State
- History
- Prediction
- Wiring Map
- Validation Status
- Hardware Blockers
- Dashboard

Vision must stay separate from execution. It may recommend actions, but
Commander and Kernel gates decide execution.

## Knowledge Layer

The Knowledge Layer stores engineering facts and lessons.

Knowledge sources:

- Datasheets
- Reference designs
- Protocols
- Pin maps
- Validation history
- Repair notes
- Known issues
- Lessons learned

Knowledge must be versioned and traceable to evidence where possible.

## Digital Twin

The Digital Twin mirrors device and lab state.

Digital Twin state:

- Device state
- Pin state
- UART state
- SPI state
- I2C state
- Health
- Telemetry
- Firmware version
- Validation history

The twin should support simulation before hardware execution.

## Validation Engine

The Validation Engine runs reproducible validation recipes.

Supported artifacts:

- YAML/JSON recipes
- Session records
- PASS/FAIL reports
- JSONL events
- Markdown reports
- HTML reports
- Simulation results before execution

Validation flow:

```text
Load Recipe
Validate Hardware Registry
Simulate Expected Sequence
Acquire Resources
Execute Probe Commands
Capture Events
Compare Against Expected Results
Generate Report
Release Resources
```

## Hardware Validation Strategy

Flipper GPIO:

- Observe PC0 and approved pins through Probe GPIO monitor.
- Validate LOW/HIGH/LOW/restore sequences.
- Reject unsafe wiring and unknown circuits.

Flipper UART TX/RX:

- Use Probe UART as structured validation endpoint.
- Capture payload, byte count, timing, errors, and session id.
- Validate `ASTRAEON UART TEST 001` and future RX payloads.

ESP32-S3:

- Use as future embedded SDK target and comparison node.
- Validate UART/GPIO first, then WiFi and sensor modules.

W5500:

- Validate SPI wiring and Ethernet packet path.
- Begin with passive/diagnostic mode before active traffic generation.

WT32-ETH01:

- Validate Ethernet node behavior, MQTT bridge, and Node01 integration.

## Repository Blueprint

Recommended future structure:

```text
ASTRAEON/
|-- Kernel/
|-- Core/
|-- AI/
|-- Hardware/
|   |-- Probe/
|   |-- Drivers/
|   |-- Validation/
|   |-- DigitalTwin/
|   `-- Boards/
|-- Commander/
|-- Knowledge/
|-- Vision/
|-- Automation/
|-- Network/
|-- Security/
|-- Robotics/
|-- SDK/
|-- Plugins/
|-- Labs/
|-- Docs/
`-- Universe/
```

## Architecture Boundary

Do not mix Probe firmware into RogueMaster.

RogueMaster remains a reference target. Probe becomes external validation and
debug infrastructure.
