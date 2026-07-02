# ASTRAEON Domain Map

## Domain Ownership

| Domain | Owns | Extends ASTRAEON-X Through | Initial Maturity |
| --- | --- | --- | --- |
| ASTRAEON-X | Runtime, SDK, events, logger, persistence, validation, CI, auto release. | Runtime API, Event API, SDK, Plugin API. | L3 |
| Hardware | Probe nodes, buses, validation boards, logic capture, packet logger. | Device API, Hardware API, events, hardware registry. | L2 |
| Business | OCR, POS, accounting, inventory, cashflow, dashboards. | Business API, event store, data registry. | L1 |
| Communication | LINE, notifications, webhooks, MQTT notifications. | Communication API, event gateway, user registry. | L1 |
| Commander | CLI, portals, dashboards, workflow orchestration. | Commander API, Event API, Policy Engine. | L1 |
| Knowledge | Datasheets, protocols, pin maps, lessons, repair notes. | Knowledge API, semantic search, memory fabric. | L1 |
| Vision | Topology, wiring, graphs, blockers, validation state. | Vision API, Digital Twin, Universe Graph. | L1 |
| AI | Reasoning, planning, diagnostics, agents, tool calling. | AI API, Knowledge API, Event API. | L1 |
| Infrastructure | Node01, Docker, MQTT, Grafana, storage, backup. | Infrastructure API, telemetry events. | L1 |
| Media | FX3, DJI, streaming, content pipeline, media evidence. | Media API, event-linked storage. | L0 |
| Home | Home Assistant, ESP32, sensors, environment automation. | Home API, MQTT events, device registry. | L0 |
| Robotics | Drones, PLC, motors, actuators, motion safety. | Robotics API, Digital Twin, policy gates. | L0 |
| Laboratory | Validation, certification, stress tests, fixtures. | Validation Engine, reports, hardware registry. | L2 |
| Security | RBAC, secrets, audit, policy, trust boundaries. | Security API, Policy Engine, Identity Engine. | L1 |
| Cloud | OpenAI, Ollama, weather, maps, backup. | Cloud API, secure connectors. | L0 |
| Data | Event store, telemetry, registries, business DB, memory. | Data API, persistence, Time Machine. | L2 |
| DigitalTwin | Live state, health, firmware version, validation history. | DigitalTwin API, event replay. | L1 |
| Developer | SDK, CLI, plugin SDK, templates, compatibility tools. | Developer API, CI, release automation. | L2 |

## Domain Rules

- Each feature belongs to one primary domain.
- Cross-domain integration must use APIs, events, registries, and policy gates.
- Existing ASTRAEON-X systems are preserved unless Architect approves a migration.
- Hardware adapters remain thin.
- Controllers own workflow and session behavior.
- AI and Vision may recommend actions; Kernel and Commander gate execution.
