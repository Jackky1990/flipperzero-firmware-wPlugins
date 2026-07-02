# ASTRAEON Handoff

## Current State

R8E-3 UART TX Hardware Validation Package is complete and verified.

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

- Controller TX workflow only.
- No RX.
- No `furi_hal_serial_init`.
- No `furi_hal_serial_deinit`.
- No DMA.
- No IRQ/callbacks.
- No worker threads.
- No stream buffers.
- No polling.
- No demo UI integration.
- No USB CDC changes.
- No expansion service changes.
- No firmware-core changes.
- No hardware validation execution was performed in this documentation package phase.

## Next Task

Execute R8E-4 UART TX Hardware Validation after Architect approval and with known-good USB-UART hardware connected.
