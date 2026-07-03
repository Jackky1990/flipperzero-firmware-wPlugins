# ASTRAEON Home Capability

| Capability | Status | Dependencies | Next Milestone |
| --- | --- | --- | --- |
| Home Assistant | L1 Architecture | MQTT, sensors, Node01 | Home registry plan. |
| Environment | L0 Concept | sensors, telemetry | Environment data model. |
| ESP32 | L0 Concept | firmware, MQTT, sensors | ESP32 node architecture. |
| Energy | L0 Concept | sensors, dashboard | Energy telemetry scope. |
| Security | L0 Concept | sensors, policy, alerts | Home security boundary. |
| Automation | L0 Concept | policy, Home Assistant | Safe home automation rules. |

## Home Rules

- Home automations require explicit policy.
- Environment observations are preferred before control actions.
