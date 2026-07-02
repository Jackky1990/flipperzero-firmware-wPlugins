# Next Action

R8E-2 UART Controller TX Workflow is complete. The demo runtime controller now owns UART TX validation flow for an already-active UART session and uses the thin Flipper serial adapter write primitive without adding RX, DMA, IRQ callbacks, workers, polling, stream buffers, UI integration, USB CDC changes, expansion service changes, or firmware-core changes.

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

- Need LED + 330Ω/1kΩ resistor or validated hardware test board.
- Do not continue GPIO hardware write validation with the damaged legacy board.

## Remaining Work

- Prepare ASTRAEON Hardware Validation Board plan.
- Define safe PC0 validation hardware before resuming controlled GPIO write validation.
- Keep PC0 as the only validation target unless Architect changes the pin policy.
- Plan R8E-3 UART TX Hardware Validation after Architect approval.
- Keep R8E separated from RX, DMA, IRQ/callbacks, worker threads, stream buffers, USB CDC changes, expansion service changes, and firmware-core changes unless explicitly approved.

## Next Implementation Step

Prepare R8E-3 UART TX Hardware Validation Plan. Validate the TX workflow with explicit hardware setup, serial ownership rules, event/log evidence, and rollback criteria before any RX implementation begins.
