# Next Action

R8D is complete. ASTRAEON now has a controller-owned UART session lifecycle for open, active, close, cancel, and timeout paths. It validates config before acquire, rejects duplicate sessions, releases exactly once after acquire, and records best-effort log/event evidence through the existing diagnostics path.

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

## Blocker

- Need LED + 330Ω/1kΩ resistor or validated hardware test board.
- Do not continue GPIO hardware write validation with the damaged legacy board.

## Remaining Work

- Prepare ASTRAEON Hardware Validation Board plan.
- Define safe PC0 validation hardware before resuming controlled GPIO write validation.
- Keep PC0 as the only validation target unless Architect changes the pin policy.
- Prepare R8E UART Read/Write Implementation Plan after Architect approval.
- Keep R8E separated from DMA, IRQ/callbacks, worker threads, stream buffers, USB CDC changes, expansion service changes, and firmware-core changes unless explicitly approved.

## Next Implementation Step

Prepare R8E UART Read/Write Implementation Plan for Architect review. Do not add UART TX/RX implementation, DMA, IRQ callbacks, worker threads, stream buffers, polling, USB CDC changes, expansion service changes, or firmware-core changes without explicit approval.
