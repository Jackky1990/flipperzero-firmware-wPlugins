# ASTRAEON Handoff

## Current State

R8F-2 UART Async RX Adapter is complete and verified.

## Completed

- Upgraded `scripts/astraeon_release.sh` with explicit `--auto` mode.
- Added safe VERIFY -> COMMIT -> PUSH -> VERIFY REMOTE SYNC flow.
- Preserved `--dry-run`.
- Made clean working tree exit successfully with `nothing to release`.
- Added release report output for branch, verification, commit, push, and sync.
- Added push failure handling with a recovery command.
- Preserved forbidden path checks and `main`/`master` rejection.
- Added `astra_flipper_serial_adapter_write`.
- Kept the adapter thin: no logging, persistence, runtime events, retries, or policy logic.
- Required an acquired serial channel before write.
- Validated adapter, channel, data, length, and `out_written`.
- Supported zero-length writes safely.
- Accounted accepted write length through `out_written`.
- Mirrored the adapter primitive into the demo SDK copy to preserve SDK drift checks.
- Added host/runtime tests for invalid arguments, invalid channel, write-before-acquire, zero-length write, and bounded write accounting.
- Added controller-owned UART TX workflow for active UART sessions.
- Added runtime TX state fields for checked/active/ok/status/run count/requested bytes/written bytes.
- Added TX begin/finish runtime helpers.
- Wired controller TX flow to `astra_flipper_serial_adapter_write`.
- Preserved existing UART session after TX.
- Added best-effort UART TX log/event persistence using the existing session id.
- Added runtime tests for inactive session, invalid session state, invalid data, zero-length TX, successful TX, and adapter failure status.
- Added TX-only UART hardware validation guide.
- Added primary UART connection diagram for 3.3V USB-UART validation.
- Documented payload `ASTRAEON UART TEST 001` and expected accepted byte count `22`.
- Documented expected terminal output, runtime diagnostics, storage evidence, failure matrix, recovery steps, and validation report template.
- Linked the UART TX validation guide from the general hardware validation checklist.
- Added a minimal `UART TX` demo screen.
- Wired the screen `OK` action to the existing controller-owned UART open -> TX -> close sequence.
- Reused payload `ASTRAEON UART TEST 001`.
- Updated the hardware validation guide with the concrete demo trigger path.
- Added UART RX runtime/session state fields.
- Added UART RX diagnostics counters for runs, expected bytes, received bytes, timeouts, overflows, and errors.
- Added RX runtime helpers for arm, record, finish, cancel, and timeout.
- Rejected inactive RX with `AstraStatusPermissionDenied`.
- Rejected duplicate RX with `AstraStatusBusy`.
- Rejected invalid UART session state with `AstraStatusInvalidArgument`.
- Reset RX active/state on UART close/finish.
- Added runtime tests for inactive session rejection, duplicate RX rejection, RX arm state, timeout, cancel, diagnostics counters, and reset on close.
- Added thin async RX primitives to the Flipper serial adapter.
- Added a fixed 256-byte RX ring buffer per adapter channel.
- Added ISR-safe byte copy into the preallocated ring buffer.
- Added RX available, read, clear, start, and stop adapter helpers.
- Added adapter RX counters for bytes received, bytes read, overflow count, and error count.
- Rejected duplicate async RX start with `AstraStatusBusy`.
- Kept duplicate async RX stop idempotent.
- Ensured adapter release stops active async RX and resets RX adapter state.
- Mirrored the adapter implementation into the demo SDK copy to preserve SDK drift.
- Added host/runtime tests for start/stop, duplicate start, duplicate stop, empty buffer, full buffer, overflow counter, clear, read ordering, and wrap-around.

## Verification

- Safe Patch Audit: PASS
- Demo FAP Build: PASS
- Public API Check: PASS
- Dependency Check: PASS
- Demo SDK Drift: PASS
- Runtime Tests: PASS
- Architecture Audit: PASS
- AEP Verify: PASS
- ASTRAEON CI: PASS

## Constraints Preserved

- Adapter RX primitive only.
- No controller workflow.
- No runtime state updates from the adapter.
- No logger.
- No persistence.
- No UI.
- No `furi_hal_serial_init`.
- No `furi_hal_serial_deinit`.
- No DMA.
- No DMA IRQ/callbacks.
- No worker threads.
- No stream buffers.
- No background polling.
- Minimal demo UI trigger only for UART TX validation.
- No USB CDC changes.
- No expansion service changes.
- No firmware-core changes.
- No hardware validation execution was performed in this trigger phase.

## Next Task

Implement R8F-3 UART RX Controller Workflow after Architect approval.
