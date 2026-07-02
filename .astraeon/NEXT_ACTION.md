# Next Action

Master Ecosystem Architecture is documented. The ASTRAEON long-term vision,
domains, kernel architecture, hardware lab, probe architecture, engineering
principles, and master roadmap are now recorded under `.astraeon/` without
firmware, SDK, runtime, or build-system behavior changes.

## Architecture Status

Approved. The current device HAL flow is:

- ASTRAEON Device HAL contracts
- Flipper GPIO adapter skeleton
- Flipper GPIO resource binding for allowed external GPIO pins
- Demo runtime validation for GPIO adapter init and resource binding
- User-triggered GPIO input-mode read validation for `gpio_ext_pc0`
- Temporary input mode setup with `GpioModeInput`, `GpioPullNo`, `GpioSpeedLow`
- Conditional restore to `GpioModeAnalog`, `GpioPullNo`, `GpioSpeedLow`
- Adapter-only controlled output primitives:
  - `astra_flipper_gpio_adapter_set_output_low_mode`
  - `astra_flipper_gpio_adapter_write_pin`
- Controller-owned controlled GPIO write session for PC0
- Explicit confirmation before output mode/write behavior
- Runtime session id format: `EVT-FLP-GPIO-WRITE-*`
- Write session steps: `begin`, `output-low`, `output-high`, `output-low-final`, `restore`, `end`
- Restore only when the controller changed the pin mode
- No interrupt registration
- No automatic GPIO action during app startup
- No GPIO write during screen rendering
- No background GPIO write or polling
- No NFC, RFID, IR, SubGHz, BLE, USB, or Serial adapter work in this phase
- UART contract foundation for Embedded SDK v1:
  - `AstraDeviceSerialConfig`
  - `AstraDeviceSerialStatus`
  - `AstraDeviceSerialDiagnostics`
  - `AstraDeviceSerialSession`
  - validation only; no hardware behavior
- Thin Flipper UART adapter skeleton:
  - primary and secondary channel mapping
  - baud allowlist validation
  - acquire/release lifecycle
  - idempotent release
  - busy-state query
  - minimal adapter state
  - no TX/RX, DMA, IRQ/callback, worker, polling, logging, persistence, or runtime workflow
- Controller-owned UART session lifecycle:
  - runtime UART session state
  - open/close controller functions
  - cancel/timeout handling
  - duplicate session rejection
  - invalid config rejection before acquire
  - acquire/release lifecycle through the thin adapter
  - best-effort diagnostics event/log recording
  - no TX/RX, `furi_hal_serial_init`, `furi_hal_serial_deinit`, DMA, IRQ/callback, worker, stream buffer, polling, demo UI, USB CDC, expansion service, or firmware-core changes
- UART TX adapter primitive:
  - `astra_flipper_serial_adapter_write`
  - validates adapter, channel, data, length, and `out_written`
  - rejects write before acquire
  - supports zero-length writes with zero bytes written
  - records bounded accepted byte count
  - no RX, IRQ/callbacks, DMA, worker threads, stream buffers, polling, controller workflow, UI, USB CDC changes, expansion service changes, or firmware-core changes
- Controller-owned UART TX workflow:
  - requires an active UART session
  - uses existing UART session id for TX event/log correlation
  - records `tx-begin`
  - calls `astra_flipper_serial_adapter_write`
  - records `tx-complete` or `tx-error`
  - leaves the UART session active after TX
  - rejects inactive sessions with `AstraStatusPermissionDenied`
  - rejects invalid session state and invalid data with `AstraStatusInvalidArgument`
  - supports zero-length TX as a safe no-op with event/log semantics
  - no RX, DMA, IRQ/callbacks, worker threads, stream buffers, polling, demo UI, USB CDC changes, expansion service changes, or firmware-core changes
- UART TX hardware validation package:
  - guide path: `applications/external/astraeon/docs/UART_TX_HARDWARE_VALIDATION.md`
  - validation payload: `ASTRAEON UART TEST 001`
  - expected accepted bytes: `22`
  - target channel: ASTRAEON primary UART
  - source mapping: `AstraDeviceSerialChannelPrimary` -> `FuriHalSerialIdUsart`
  - hardware setup: Flipper TX -> USB-UART RX, common GND, 3.3V logic
  - forbidden wiring: USB-UART TX to Flipper RX, VCC, 5V logic, unknown circuits
  - evidence: terminal output, session id, bytes requested/written, release count, adapter release, logs, persistent events
  - no RX, DMA, IRQ/callbacks, workers, polling, stream buffers, USB CDC changes, expansion service changes, or firmware-core changes
- UART TX validation trigger:
  - demo screen: `UART TX`
  - user action: press `OK` once
  - sequence: open UART session, transmit `ASTRAEON UART TEST 001`, close UART session
  - reuses existing controller workflow, adapter primitive, runtime diagnostics, storage log, and persistent event path
  - no RX, DMA, IRQ/callbacks, workers, polling, stream buffers, USB CDC changes, expansion service changes, firmware-core changes, or new UART APIs
- UART RX runtime foundation:
  - RX state fields for checked, ok, active, status, and state
  - RX diagnostics counters for runs, expected bytes, received bytes, timeout count, overflow count, and error count
  - runtime helpers for arm, record diagnostics, finish, cancel, and timeout
  - inactive sessions rejected with `AstraStatusPermissionDenied`
  - duplicate RX rejected with `AstraStatusBusy`
  - invalid active session state rejected with `AstraStatusInvalidArgument`
  - RX state resets on UART close/finish
  - no Flipper HAL RX calls, async callback, DMA, IRQ, worker thread, stream buffer, polling, UI, USB CDC change, expansion service change, firmware-core change, or hardware validation
- UART async RX adapter:
  - `astra_flipper_serial_adapter_start_async_rx`
  - `astra_flipper_serial_adapter_stop_async_rx`
  - `astra_flipper_serial_adapter_rx_available`
  - `astra_flipper_serial_adapter_read`
  - `astra_flipper_serial_adapter_clear`
  - fixed 256-byte RX ring buffer per adapter channel
  - ISR callback only drains HAL RX bytes into the preallocated buffer and updates counters
  - diagnostics counters for bytes received, bytes read, overflow count, and error count
  - duplicate start rejected with `AstraStatusBusy`
  - duplicate stop is idempotent
  - release stops active async RX and resets RX adapter state
  - no controller workflow, runtime state update, logger, persistence, UI, DMA, worker thread, stream buffer, background polling, firmware-core change, USB CDC change, or expansion service change
- Controller-owned UART RX workflow:
  - requires an active UART session
  - uses existing UART session id for RX event/log correlation
  - arms async RX through `astra_flipper_serial_adapter_start_async_rx`
  - drains the bounded adapter RX buffer once
  - validates expected payload bytes when available
  - stops async RX before complete, timeout, cancel, or error
  - records RX diagnostics through runtime counters
  - leaves the UART session active after RX completion, timeout, or cancel
  - rejects inactive RX with `AstraStatusPermissionDenied`
  - reports incomplete payload as `AstraStatusTimeout`
  - reports payload mismatch as `AstraStatusProtocolError`
  - no UI integration, hardware validation execution, DMA, worker thread, stream buffer, polling, USB CDC change, expansion service change, or firmware-core change
- AEO auto release pipeline:
  - `scripts/astraeon_release.sh --auto "message"`
  - verifies repository and branch safety
  - rejects `main` and `master` unless explicitly allowed
  - rejects merge conflicts and forbidden paths
  - runs ASTRAEON verification before commit
  - commits only after green verification
  - pushes only in `--auto` mode after commit succeeds
  - verifies local and origin branch synchronization
  - exits cleanly with `nothing to release` when the working tree is clean

## Blocker

- GPIO controlled write validation needs LED + 330Ω/1kΩ resistor or validated hardware test board.
- Do not continue GPIO hardware write validation with the damaged legacy board.
- UART TX hardware execution needs known-good 3.3V USB-UART adapter and common GND setup.
- UART RX hardware validation needs Architect-approved validation plan and known-good 3.3V USB-UART hardware.

## Remaining Work

- Use `.astraeon/MASTER_CHARTER.md`, `.astraeon/ECOSYSTEM_ARCHITECTURE.md`,
  `.astraeon/ENGINEERING_PRINCIPLES.md`, `.astraeon/ROADMAP_MASTER.md`, and
  `.astraeon/DOMAINS.md` as architecture source-of-truth for future ASTRAEON
  ecosystem work.
- Prepare ASTRAEON Hardware Validation Board plan.
- Define safe PC0 validation hardware before resuming controlled GPIO write validation.
- Keep PC0 as the only validation target unless Architect changes the pin policy.
- Execute R8E-4 UART TX Hardware Validation with known-good USB-UART hardware connected.
- Prepare R8F-4 UART RX Hardware Validation Plan after Architect approval.
- Start ASTRAEON-Hardware P0 Documentation after Architect approval if the
  hardware lab work moves into a separate repository.
- Complete Embedded SDK v1.0 Release after hardware validation evidence is accepted or explicitly deferred by Architect.
- Keep R8E separated from RX, DMA, IRQ/callbacks, worker threads, stream buffers, USB CDC changes, expansion service changes, and firmware-core changes unless explicitly approved.

## Next Implementation Step

Next safe implementation depends on Architect priority: prepare ASTRAEON-Hardware P0 Documentation in a separate project, or prepare R8F-4 UART RX Hardware Validation Plan in this repository. Do not mix Probe firmware into RogueMaster.
