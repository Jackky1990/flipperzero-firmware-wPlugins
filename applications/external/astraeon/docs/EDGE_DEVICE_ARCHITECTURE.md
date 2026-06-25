# EDGE DEVICE ARCHITECTURE

## Purpose

ASTRAEON Edge Platform connects field devices to ASTRAEON-X Core.
Flipper Zero is a field terminal, not the AI brain.

## Device Roles

Mac Studio:
- main AI/media workstation
- ASTRAEON-X development control

MacBook:
- Flipper development
- RogueMaster build/test/sync

Node01:
- FastAPI runtime
- MQTT broker
- database
- dashboard
- automation engine

Flipper Zero:
- scan
- log
- authorized control
- mission terminal
- field diagnostics

ESP32-S3:
- wireless edge sensor
- relay control
- BLE/UART bridge

WT32-ETH01:
- Ethernet edge node
- MQTT relay
- LAN sensor bridge

Home Assistant:
- smart home device orchestration
- scenes and automations

## High Level Flow

Flipper -> USB/UART -> Node01 -> FastAPI/MQTT -> Home Assistant/ESP32/WT32 -> Devices

## Security Position

- All commands must pass policy.
- Unknown signals are passive log only.
- No unauthorized access testing.
- No destructive automation.
