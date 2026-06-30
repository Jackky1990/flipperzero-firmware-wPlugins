# ASTRAEON Edge Platform

ASTRAEON turns the Flipper Zero firmware tree into an ASTRAEON-X field terminal.
The Flipper app is not the AI brain; it is the edge device runtime for local
status, diagnostics, authorized control, and sync with Node01.

## Repository Boundary

- `applications/external/astraeon/sdk` is the canonical SDK source.
- `applications/external/astraeon/tests` contains host-side runtime tests.
- `applications/external/astraeon/tools` contains AEP verification tooling.
- `applications/external/astraeon_demo` is the first Flipper UI consumer.

The demo currently vendors a small SDK subset under
`applications/external/astraeon_demo/lib/astraeon_sdk` so it can build as a FAP
private library. The vendored files must match the canonical SDK byte-for-byte.
Run `python3 applications/external/astraeon/tools/aep.py check-demo-sdk` before
changing the demo SDK subset.

## Runtime Stack

The implemented runtime stack is:

1. runtime session
2. runtime handshake
3. runtime capabilities
4. runtime heartbeat
5. runtime message
6. runtime codec
7. runtime transport

Loopback transport is the default safe path. USB CDC transport exists behind
`ASTRA_RUNTIME_ENABLE_USB_CDC` and should only be enabled in a Flipper firmware
build path when the integration is ready.

## Quality Gates

Use the AEP tool from the repository root:

```sh
python3 applications/external/astraeon/tools/aep.py verify
```

For demo-facing changes, also build the FAP:

```sh
./fbt fap_astraeon_demo
```

The unified CI helper can combine both paths:

```sh
python3 applications/external/astraeon/tools/codex/astraeon_ci.py --build-demo
```
