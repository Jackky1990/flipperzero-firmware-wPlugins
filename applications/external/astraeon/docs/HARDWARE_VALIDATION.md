# ASTRAEON Hardware Validation

This checklist validates `astraeon_demo` on a connected Flipper Zero.

## Install And Launch

From the repository root:

```sh
./fbt launch APPSRC=astraeon_demo
```

Expected result:

- The FAP builds successfully.
- The FAP is installed to `/ext/apps/Tools/astraeon_demo.fap`.
- The app launches on the Flipper.

## Run Diagnostics

On the Flipper, press `OK` in ASTRAEON Demo.

Expected screen state:

```text
Run:OK Xport:OK
Cap:OK HS:OK Ping:OK
S+ L+ E+ N+ U+ P+
```

`U` may remain `-` when `ASTRAEON_DEMO_USB_CDC = 0`; safe builds keep real USB
CDC disabled by default.

## Verify Storage Evidence

Replace the port with the connected Flipper port when needed.

```sh
toolchain/arm64-darwin/bin/python3 scripts/storage.py -p /dev/cu.usbmodemflip_Ovimalur1 list /ext/astraeon
toolchain/arm64-darwin/bin/python3 scripts/storage.py -p /dev/cu.usbmodemflip_Ovimalur1 read /ext/astraeon/logs/astra.log
toolchain/arm64-darwin/bin/python3 scripts/storage.py -p /dev/cu.usbmodemflip_Ovimalur1 read /ext/astraeon/events/flipper_events.jsonl
```

Expected files:

- `/ext/astraeon/logs/astra.log`
- `/ext/astraeon/events/flipper_events.jsonl`

Expected log evidence:

```text
[INFO] diagnostics
```

Expected event evidence:

```json
{"type":"DIAGNOSTICS_REPORT","status":"OK","target_device":"NODE01"}
```

## UART TX Validation

For TX-only UART hardware validation, follow
`applications/external/astraeon/docs/UART_TX_HARDWARE_VALIDATION.md`.

This validates only the ASTRAEON primary UART TX path at `115200 8N1` with a
3.3V USB-UART adapter. It does not validate RX, DMA, IRQ callbacks, workers,
polling, stream buffers, USB CDC, or expansion services.

## Current Hardware Result

Hardware validation passed on connected port `/dev/cu.usbmodemflip_Ovimalur1`:

- ASTRAEON Demo installed and launched.
- Runtime diagnostics wrote `/ext/astraeon/logs/astra.log`.
- Runtime diagnostics wrote `/ext/astraeon/events/flipper_events.jsonl`.
- Two diagnostics events were observed with `status` set to `OK` and
  `target_device` set to `NODE01`.
