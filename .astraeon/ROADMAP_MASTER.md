# ASTRAEON Master Roadmap

## Phase 0: Foundation And Documentation

Goal:

- Establish master charter, ecosystem architecture, engineering principles,
  domains, and roadmap.

Exit criteria:

- Core documents exist.
- Execution rules are explicit.
- Repository boundaries are clear.

## Phase 1: Kernel Foundation

Goal:

- Implement Event Kernel, Device Kernel, Session Kernel, Resource Kernel,
  Permission Kernel, Scheduler, Logger, Telemetry, Diagnostics, Recovery, and
  API Gateway foundations.

Exit criteria:

- Kernel can model sessions, resources, permissions, events, and health without
  target-specific behavior.

## Phase 2: Probe Core

Goal:

- Create ASTRAEON Probe as a separate hardware project.

Exit criteria:

- Probe boots, reports version/capabilities, exposes USB CDC, runs self-test,
  and emits JSONL events.

## Phase 3: UART

Goal:

- Implement Probe UART bridge and validation endpoint.

Exit criteria:

- Probe validates Flipper UART TX/RX with structured session records and
  PASS/FAIL reports.

## Phase 4: GPIO

Goal:

- Implement Probe GPIO monitor and safe validation recipes.

Exit criteria:

- Probe observes Flipper GPIO controlled validation without unsafe drive.

## Phase 5: Packet Logger

Goal:

- Add structured packet/event capture for UART and future buses.

Exit criteria:

- Commander can store session logs, packet streams, dropped counters, and
  reports.

## Phase 6: Validation Engine

Goal:

- Add YAML/JSON validation recipes and deterministic PASS/FAIL generation.

Exit criteria:

- Recipes simulate first, execute through Commander, and produce JSONL plus
  Markdown/HTML reports.

## Phase 7: Logic Capture

Goal:

- Add bounded logic capture for GPIO and bus timing.

Exit criteria:

- Probe captures timestamped transitions and exports evidence for diagnostics.

## Phase 8: Multi-Probe Network

Goal:

- Support multiple Probe nodes over USB, WiFi, or Ethernet.

Exit criteria:

- Commander discovers probes, assigns roles, and correlates events by session.

## Phase 9: Digital Twin

Goal:

- Model devices, pins, buses, firmware versions, health, telemetry, and
  validation history.

Exit criteria:

- Digital Twin can simulate validation recipes before execution.

## Phase 10: AI Diagnostics

Goal:

- Use AI to diagnose failures from logs, events, captures, knowledge, and
  Digital Twin state.

Exit criteria:

- AI generates grounded root-cause hypotheses and minimal fix proposals.

## Phase 11: Automation

Goal:

- Add safe automation hooks for repeatable lab validation and infrastructure
  workflows.

Exit criteria:

- Automation is permissioned, observable, logged, and reversible where possible.

## Phase 12: Remote Hardware Lab

Goal:

- Expose controlled remote validation through Node01 and secure networking.

Exit criteria:

- Remote sessions can run validation recipes with identity, permissions, and
  audit trail.

## Phase 13: Robotics Integration

Goal:

- Integrate robotics controllers, sensors, and physical actuation into the
  same event/session/resource architecture.

Exit criteria:

- Robotics actions require simulation, permission, validation, telemetry, and
  recovery.

## Phase 14: Autonomous Laboratory

Goal:

- Combine AI diagnostics, Digital Twin, Probe Network, validation recipes, and
  automation into a controlled autonomous lab.

Exit criteria:

- ASTRAEON can propose, simulate, execute, observe, diagnose, and report safe
  hardware experiments under explicit policy gates.

## Current Near-Term Roadmap

- R8F-4 UART RX Hardware Validation Plan.
- UART TX Hardware Execution.
- GPIO Hardware Validation.
- Embedded SDK v1.0 Release.
- ASTRAEON-Hardware P0 Documentation.
- ASTRAEON Probe P1 UART Bridge.
