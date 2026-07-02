# ASTRAEON UART TX Hardware Validation

This guide validates ASTRAEON UART TX only. It does not validate RX, DMA, IRQ
callbacks, workers, polling, stream buffers, USB CDC, or expansion services.

## Validation Target

- Device: Flipper Zero
- Channel: ASTRAEON primary UART
- Flipper mapping: `AstraDeviceSerialChannelPrimary` -> `FuriHalSerialIdUsart`
- UART config: `115200` baud, `8N1`, no flow control
- Payload: `ASTRAEON UART TEST 001`
- Expected accepted bytes: `22`
- Scope: acquire, open session, configure, TX payload, release, diagnostics

Primary UART uses the USART external header pins documented in this repository
as pin `13` and pin `14`. The TX-only validation connects Flipper TX to the
USB-UART adapter RX. Do not connect USB-UART TX to Flipper RX for this phase.

## Required Hardware

- Flipper Zero running the current `astraeon_demo` build.
- USB-UART adapter set to `3.3V` logic.
- Three jumper wires.
- Host terminal capable of reading the USB-UART adapter at `115200 8N1`.

Do not use a `5V` UART adapter. Do not connect VCC between the adapter and the
Flipper. Do not connect unknown circuits, motors, relays, or powered modules.

## Connection Diagram

```text
Flipper external header                 USB-UART adapter

Pin 13 / USART TX  -------------------> RX
GND                 -------------------> GND

Do not connect:
Pin 14 / USART RX  <------------------- TX
3V3 / 5V / VCC
```

## Terminal Setup

Find the USB-UART adapter device on the host, then open it at `115200 8N1`.

Example:

```sh
ls /dev/cu.*
screen /dev/cu.usbserial-XXXX 115200
```

Expected terminal output after the validation TX step:

```text
ASTRAEON UART TEST 001
```

If the terminal does not show a trailing newline, that is acceptable. The
payload itself must match exactly.

## Validation Procedure

1. Verify the repository build is green:

   ```sh
   python3 applications/external/astraeon/tools/aep.py verify
   ./fbt fap_astraeon_demo
   ```

2. Install and launch the demo:

   ```sh
   ./fbt launch APPSRC=astraeon_demo
   ```

3. Prepare the USB-UART terminal:

   ```sh
   screen /dev/cu.usbserial-XXXX 115200
   ```

4. Trigger the approved UART TX validation path:

   - Launch `ASTRAEON Demo`.
   - Press `Down` until the `UART TX` screen is visible.
   - Press `OK` once.

5. Confirm the controller sequence:

   ```text
   acquire
   open
   configure
   tx-begin
   tx-complete
   close
   release
   ```

6. Confirm diagnostics:

   - UART session id is nonzero.
   - `uart_session_active` becomes active during TX.
   - `uart_tx_checked` is true.
   - `uart_tx_ok` is true.
   - `uart_tx_bytes_requested` is `22`.
   - `uart_tx_bytes_written` is `22`.
   - `uart_release_count` is `1`.
   - Adapter channel is released after close.

7. Confirm storage evidence:

   ```sh
   toolchain/arm64-darwin/bin/python3 scripts/storage.py -p /dev/cu.usbmodemXXXX read /ext/astraeon/logs/astra.log
   toolchain/arm64-darwin/bin/python3 scripts/storage.py -p /dev/cu.usbmodemXXXX read /ext/astraeon/events/flipper_events.jsonl
   ```

Expected log evidence:

```text
uart session=EVT-FLP-UART-TX-<id> step=tx-begin status=OK
uart session=EVT-FLP-UART-TX-<id> step=tx-complete status=OK
```

Expected persistent event evidence:

```json
{"type":"DIAGNOSTICS_REPORT","status":"OK"}
```

The event id must start with `EVT-FLP-UART-TX-` and use the same session id as
the TX log lines.

## Failure Matrix

| Failure | Expected Behavior | Recovery |
| --- | --- | --- |
| USB-UART terminal shows nothing | TX may not be connected to adapter RX, wrong port, wrong baud, or validation path not triggered | Stop validation, check GND, check pin 13 -> RX, reopen terminal at `115200` |
| Garbled output | Wrong baud, wrong logic level, or unstable ground | Stop validation, verify `115200 8N1`, verify `3.3V`, reconnect GND |
| Adapter acquire fails | Controller must report busy or error and must not TX | Close other UART apps, restart demo, retry once |
| TX accepted bytes is not `22` | Controller must report TX error or incomplete diagnostics | Stop validation and capture logs/events |
| Release count is not `1` | Validation fails; UART ownership may be unsafe | Close demo, reboot Flipper if needed, report root cause |
| Storage evidence missing | Runtime persistence path failed or storage unavailable | Capture screen/runtime status, inspect SD card, rerun diagnostics |
| Flipper freezes or resets | Validation fails | Disconnect USB-UART, reboot, do not retry until root cause is reviewed |

## Recovery Steps

1. Exit `astraeon_demo`.
2. Disconnect the USB-UART adapter from the Flipper header.
3. Power-cycle the Flipper if UART ownership does not release cleanly.
4. Re-run:

   ```sh
   python3 applications/external/astraeon/tools/aep.py verify
   ```

5. Preserve:

   - terminal capture
   - `/ext/astraeon/logs/astra.log`
   - `/ext/astraeon/events/flipper_events.jsonl`
   - exact session id
   - observed payload

## Validation Report Template

```text
Validation ID:
Date:
Firmware commit:
Flipper:
USB-UART adapter:
UART config: 115200 8N1
Payload: ASTRAEON UART TEST 001

Connections:
- Flipper pin 13 USART TX -> USB-UART RX:
- Flipper GND -> USB-UART GND:
- USB-UART TX disconnected:
- VCC disconnected:

Observed terminal output:

Session:
- session id:
- bytes requested:
- bytes written:
- release count:
- adapter released:

Storage evidence:
- astra.log:
- flipper_events.jsonl:

Result: PASS / FAIL
Root cause if FAIL:
Recovery performed:
```

## Pass Criteria

The validation passes only if all of the following are true:

- The host terminal shows `ASTRAEON UART TEST 001`.
- The TX session uses one nonzero session id.
- TX begins only after the UART session is active.
- `uart_tx_bytes_requested == 22`.
- `uart_tx_bytes_written == 22`.
- TX completes with `AstraStatusOk`.
- The UART session closes cleanly.
- Release is executed exactly once.
- Adapter state is not busy after release.
- Logs and persistent events contain matching UART TX evidence.
