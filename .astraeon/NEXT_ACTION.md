# Next Action

R8C is complete. ASTRAEON now has a thin Flipper UART adapter skeleton with channel mapping, allowlist baud validation, acquire/release ownership state, busy-state query, and host/runtime tests. It does not implement TX/RX transfer, DMA, IRQ callbacks, worker threads, stream buffers, polling, demo UI integration, runtime sessions, USB CDC changes, or firmware-core changes.

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

## Blocker

- Need LED + 330Ω/1kΩ resistor or validated hardware test board.
- Do not continue GPIO hardware write validation with the damaged legacy board.

## Remaining Work

- Prepare ASTRAEON Hardware Validation Board plan.
- Define safe PC0 validation hardware before resuming controlled GPIO write validation.
- Keep PC0 as the only validation target unless Architect changes the pin policy.
- Design R8D UART Controller Session workflow after Architect approval.
- Keep R8D separate from UART TX/RX data transfer unless explicitly approved.

## Next Implementation Step

Prepare R8D UART Controller Session Design for Architect review. Do not add UART TX/RX data transfer, DMA, IRQ callbacks, worker threads, stream buffers, polling, USB CDC changes, expansion service changes, or firmware-core changes without explicit approval.
