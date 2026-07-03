# ASTRAEON Hardware Capability

| Capability | Status | Dependencies | Next Milestone |
| --- | --- | --- | --- |
| GPIO | L3 Software | Flipper SDK, controller, tests | Known-good hardware validation fixture. |
| UART | L3 Software | Serial adapter, controller, tests | TX/RX hardware validation. |
| SPI | L0 Concept | Probe, W5500, recipes | SPI architecture audit. |
| I2C | L0 Concept | Probe, sensors, recipes | I2C architecture audit. |
| CAN | L0 Concept | transceiver, Probe, validation board | CAN scope decision. |
| BLE | L0 Concept | Flipper/ESP32 capabilities | BLE architecture audit. |
| SubGHz | L0 Concept | Flipper APIs, RF safety | SubGHz scope and safety review. |
| IR | L0 Concept | Flipper APIs | IR adapter architecture. |
| NFC | L0 Concept | Flipper APIs | NFC adapter architecture. |
| Probe | L1 Architecture | ESP32-S2, USB CDC, WiFi | ASTRAEON-Hardware P0 docs. |
| Logic Capture | L0 Concept | Probe buffer, timing, reports | Logic capture design. |
| Validation | L3 Software | AEP, runtime tests, docs | Hardware validation board. |

## Hardware Rules

- Observe before control.
- No automatic hardware side effects.
- Hardware validation requires session id, evidence, and restore policy.
