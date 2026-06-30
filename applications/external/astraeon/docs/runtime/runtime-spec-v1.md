# ASTRAEON Runtime Specification v1.0

## Status

Draft Freeze v1.0

## Source Of Truth

`applications/external/astraeon/sdk` is the canonical runtime SDK.
`applications/external/astraeon_demo/lib/astraeon_sdk` may contain a vendored
subset for FAP-private builds, but those files must remain byte-for-byte equal
to the canonical SDK files.

## Quality Gate

Every runtime/protocol change must pass:

- Firmware build
- Runtime tests
- Contract scanner
- Architecture audit
- Demo SDK drift check
- Git Guard

## Protocol Stack

```text
Runtime Session
Runtime Handshake
Runtime Capabilities
Runtime Heartbeat
Runtime Message
Runtime Codec
Runtime Transport
```

## Transport Policy

Loopback transport is the default runtime test and demo transport. Hardware
transport must stay behind runtime transport APIs. USB CDC is disabled by
default through `ASTRA_RUNTIME_ENABLE_USB_CDC`.

## Demo Policy

`astraeon_demo` should exercise the runtime through SDK APIs instead of
reimplementing runtime behavior in UI code. The UI may display runtime status,
transport capabilities, handshake state, heartbeat state, and ping results, but
it must not call HAL or storage APIs directly.
