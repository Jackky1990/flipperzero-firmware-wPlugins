# ASTRAEON Module Registry

## Modules

| Module | Layer | Owner | Current State | Dependencies | Registry |
| --- | --- | --- | --- | --- | --- |
| Runtime Module | Kernel | ASTRAEON-X | Implemented | SDK, events | Preservation Registry |
| GPIO Module | Hardware | ASTRAEON-X | Software validated | Device HAL, controller | Preservation Registry |
| UART Module | Hardware | ASTRAEON-X | Software validated | Serial adapter, controller | Preservation Registry |
| SPI Module | Hardware | ASTRAEON-Hardware | Concept | Probe, SDK | Hardware Capability |
| I2C Module | Hardware | ASTRAEON-Hardware | Concept | Probe, SDK | Hardware Capability |
| CAN Module | Hardware | ASTRAEON-Hardware | Concept | Probe, transceiver | Hardware Capability |
| BLE Module | Hardware | ASTRAEON-X/Hardware | Concept | Flipper/ESP32 | Hardware Capability |
| SubGHz Module | Hardware | ASTRAEON-X | Concept | Flipper APIs | Hardware Capability |
| IR Module | Hardware | ASTRAEON-X | Concept | Flipper APIs | Hardware Capability |
| NFC Module | Hardware | ASTRAEON-X | Concept | Flipper APIs | Hardware Capability |
| Probe Module | Hardware | ASTRAEON-Hardware | Architecture | ESP32-S2 | Hardware Capability |
| Logic Capture Module | Laboratory | ASTRAEON-Hardware | Concept | Probe, buffers | Hardware Capability |
| Validation Module | Laboratory | ASTRAEON-X | Software validated | AEP, CI | Preservation Registry |
| Portal Module | Commander | Commander | Architecture | Streamlit/FastAPI | Portal Registry |
| Business Module | Business | Business | Architecture | OCR, DB, dashboards | Business Capability |
| Communication Module | Communication | Communication | Architecture | LINE, MQTT | Communication Capability |
| AI Module | AI | AI | Architecture | Knowledge, Data | AI Capability |
| Knowledge Module | Knowledge | Knowledge | Architecture | docs, search | Knowledge Capability |
| Media Module | Media | Media | Concept | FX3, DJI, storage | Media Capability |
| Home Module | Home | Home | Concept | Home Assistant, ESP32 | Home Capability |

## Module Rules

- Modules declare owner, layer, dependencies, APIs, events, and tests before
  implementation.
- A module may depend on ASTRAEON-X but must not rewrite protected assets.
