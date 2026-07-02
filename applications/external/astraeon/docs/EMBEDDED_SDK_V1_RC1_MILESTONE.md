# ASTRAEON Embedded SDK v1.0 RC1 Milestone

## Purpose

This document freezes the current ASTRAEON Flipper platform software
architecture as Embedded SDK v1.0 RC1.

RC1 is a release-candidate architecture milestone. It confirms that the
software foundation is green and that remaining work is limited to approved
hardware validation and release packaging.

## Scope

Included:

- ASTRAEON SDK contracts and runtime foundation.
- Flipper GPIO adapter foundation and controller-managed validation workflow.
- Flipper UART contract, adapter, controller session, TX workflow, and RX
  workflow foundation.
- Event, logger, persistence, CI, and auto release workflow.

Excluded:

- New firmware behavior.
- Firmware-core modifications.
- USB CDC changes.
- Expansion service changes.
- DMA, worker-thread, stream-buffer, or background-polling UART work.
- Hardware validation execution that is currently blocked by missing hardware.

## Component Matrix

| Component | RC1 Status | Implemented Scope | Remaining Work | Release Risk |
| --- | --- | --- | --- | --- |
| GPIO | Software complete, hardware validation partially blocked | Device HAL contracts, Flipper adapter skeleton, resource binding, read validation, input-mode setup, controlled write session, safe restore policy | Complete controlled write hardware validation with known-good LED/resistor or validated board | Medium |
| UART | Software foundation complete, hardware validation pending | Serial contracts, thin Flipper adapter, acquire/release, TX primitive, TX controller workflow, TX validation trigger, async RX adapter, RX controller workflow | Execute TX hardware validation, plan and execute RX hardware validation | Medium |
| Runtime | Green | Runtime loop, context, manager, session, isolation, transport, handshake, capabilities, heartbeat, message, ping, UART/GPIO workflow state | Continue regression coverage as hardware paths mature | Low |
| Event | Green | Event model, builder, queue, bus, lifecycle, source, id/types, diagnostics event flow | Keep event schema stable through v1.0 | Low |
| Logger | Green | Storage-backed logger, diagnostics records, UART/GPIO workflow step logs | Confirm hardware validation evidence on device storage | Low |
| Persistence | Green | Persistent diagnostics events, storage adapter, event append path, hardware evidence path | Confirm UART RX/TX and GPIO hardware events on real devices | Low |
| CI | Green | Safe Patch Audit, Public API, Dependency Check, SDK Drift, Runtime Tests, Demo FAP, Architecture Audit, AEP Verify, ASTRAEON CI | Keep CI as release gate for v1.0 | Low |
| Auto Release | Green | `scripts/astraeon_release.sh --auto` verifies, commits, pushes, and confirms origin sync | Use for all RC and v1.0 release-state commits | Low |

## Completed Features

### SDK and Runtime

- SDK foundation: status, result, version, build info, core device contracts.
- Runtime tests: 33 host/runtime tests passing.
- Runtime architecture audit: 100 percent.
- Node01 USB bridge and runtime transport foundations.
- Policy, storage, logger, and persistent event layers.

### GPIO

- Device GPIO contract foundation.
- Thin Flipper GPIO adapter skeleton.
- Side-effect-free GPIO resource binding for allowlisted external GPIO pins.
- User-triggered GPIO read validation.
- Temporary input-mode setup for read-only validation.
- Restore to analog mode when controller changed pin mode.
- Adapter-level output primitives.
- Controller-owned controlled GPIO write session.
- Explicit confirmation before output validation.
- No startup write, render write, background write, polling, or interrupts.

### UART

- Serial contract foundation:
  - config
  - status
  - diagnostics
  - session validation
- Thin Flipper UART adapter:
  - channel mapping
  - baud allowlist
  - acquire/release
  - busy-state query
- Controller-owned UART session lifecycle:
  - open
  - acquire
  - configure
  - active
  - close
  - cancel
  - timeout
- UART TX:
  - adapter write primitive
  - controller-owned TX workflow
  - TX hardware validation guide
  - minimal demo TX validation trigger
- UART RX:
  - runtime RX state and counters
  - async RX adapter with fixed 256-byte ring buffer
  - ISR-safe byte copy into preallocated buffer
  - RX available/read/clear helpers
  - controller-owned arm/drain/cancel/timeout workflow
  - payload validation path

### Verification and Release Automation

- Verification profiles and unified CI.
- AEP contract scan.
- Architecture audit.
- Safe auto release pipeline.
- Local and remote sync verification after auto push.

## Blocked Hardware Validations

### GPIO Controlled Write Hardware Validation

Status: blocked.

Blocker:

- Need LED plus 330 ohm or 1k ohm resistor, or a validated hardware test board.
- Legacy board is damaged and must not be used.

Required validation:

- PC0 only.
- Output LOW before output mode.
- Controlled HIGH.
- Controlled LOW final.
- Restore to analog mode.
- Confirm no duplicate session and no startup/render/background writes.

### UART TX Hardware Execution

Status: blocked until known-good USB-UART hardware is available.

Required hardware:

- Flipper Zero.
- 3.3V USB-UART adapter.
- Common GND.
- Flipper TX to USB-UART RX.
- 115200 baud, 8N1.

Expected payload:

```text
ASTRAEON UART TEST 001
```

### UART RX Hardware Validation

Status: not executed.

Required before execution:

- Architect-approved R8F-4 validation plan.
- Known-good 3.3V USB-UART adapter.
- Safe validation trigger or approved diagnostics path.
- Clear expected payload and timeout policy.

## Remaining Roadmap

1. UART TX Hardware Execution
   - Execute existing TX validation guide with known-good 3.3V USB-UART adapter.
   - Capture terminal output, session id, logs, persistent events, and release
     evidence.

2. UART RX Hardware Validation
   - Prepare R8F-4 validation plan.
   - Add only the minimum approved trigger or validation package if required.
   - Validate async RX buffer, controller drain, timeout, cancel, diagnostics,
     event, log, and persistence path.

3. GPIO Hardware Validation
   - Prepare or acquire known-good LED/resistor board.
   - Resume R7D-5C controlled GPIO write validation only after hardware is safe.

4. Embedded SDK v1.0 Release
   - Freeze public SDK surface.
   - Confirm hardware validation evidence.
   - Tag release candidate or v1.0 release.
   - Publish release notes and final validation report.

## Release Readiness Assessment

RC1 readiness: conditionally ready.

Software readiness:

- PASS. The current software architecture is green and verified.

Hardware readiness:

- NOT COMPLETE. UART TX, UART RX, and GPIO controlled write hardware validation
  remain open.

API readiness:

- PASS for current embedded SDK foundation. No public SDK redesign is required
  for RC1.

Operational readiness:

- PASS. Auto release workflow can verify, commit, push, and confirm remote sync.

Release gate for Embedded SDK v1.0:

- All RC1 software checks must remain green.
- UART TX hardware validation must pass.
- UART RX hardware validation must pass or be explicitly deferred by Architect.
- GPIO controlled write hardware validation must pass or be explicitly deferred
  by Architect.
- Hardware evidence must be recorded in AEO handoff/state or a release report.

## RC1 Decision

Embedded SDK v1.0 RC1 is suitable as a software architecture freeze.

It is not yet a final hardware-validated v1.0 release.
