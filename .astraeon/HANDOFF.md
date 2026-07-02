# ASTRAEON Handoff

## Current State

AEO Auto Release Pipeline upgrade is complete and verified.

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

- Automation only for this upgrade.
- No firmware logic changes.
- No SDK behavior changes.
- No runtime behavior changes.
- No Flipper HAL changes.
- TX adapter primitive only.
- No controller TX workflow.
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

## Next Task

Implement R8E-2 UART Controller TX Workflow after Architect approval.
