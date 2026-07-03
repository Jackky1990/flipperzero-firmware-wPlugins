# ASTRAEON Device Registry

## Known Devices

| Device | Capabilities | Protocols | Firmware | Owner | Repository | Status |
| --- | --- | --- | --- | --- | --- | --- |
| Flipper Zero | GPIO, UART, NFC, RFID, IR, SubGHz, BLE, USB, storage, buttons. | USB, UART, GPIO, RF protocols. | RogueMaster + ASTRAEON Demo. | ASTRAEON-X | RogueMaster | Reference target. |
| ESP32-S2 Probe | UART bridge, GPIO monitor, WiFi, USB CDC, OTA candidate. | USB CDC, WiFi, UART, GPIO, I2C, SPI. | Future Probe firmware. | ASTRAEON-Hardware | Future `astraeon-hardware` | Planned. |
| ESP32-S3 | Edge node, sensors, AI edge candidate. | WiFi, BLE, USB, UART, GPIO, I2C, SPI. | Future embedded firmware. | Hardware/Home | Future | Planned. |
| WT32 | Ethernet edge node. | Ethernet, UART, GPIO. | Future firmware. | Infrastructure/Hardware | Future | Planned. |
| W5500 | Ethernet module. | SPI, Ethernet. | Module firmware none. | Hardware | Future | Planned. |
| Node01 | Service host, Docker, MQTT, Grafana, storage. | SSH, Docker, HTTP, MQTT. | Ubuntu services. | Infrastructure | Node01 services | Preserved concept. |
| Mac Studio | Development workstation. | SSH, Git, local tools. | macOS. | Developer | Local | Available. |
| MacBook | Development workstation. | SSH, Git, local tools. | macOS. | Developer | Local | Available. |
| FX3 | Camera/media capture. | USB/media protocols. | Device firmware. | Media | Future | Planned. |
| DJI | Drone/media source. | DJI protocols, media transfer. | DJI firmware. | Media/Robotics | Future | Planned. |
| AW300 | Known device placeholder. | Unknown until inspected. | Unknown. | Hardware/Home | Future | Needs inspection. |
| Brother | Printer/label/device placeholder. | USB/network printing. | Device firmware. | Operations/Media | Future | Needs inspection. |
| Arduino | Microcontroller reference device. | UART, GPIO, I2C, SPI. | Sketch firmware. | Hardware/Developer | Future | Planned. |
| Future Devices | Expansion devices and boards. | Declared per device. | Declared per device. | Device owner | Device repo | Registered before use. |

## Device Rules

- Unknown capabilities must remain unknown until inspected.
- Hardware control requires owner, protocol, firmware, policy, and validation
  status.
- Devices become resources before automation can use them.
