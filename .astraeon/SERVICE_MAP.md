# ASTRAEON Service Map

## Core Services

| Service | Domain | Purpose | Integration |
| --- | --- | --- | --- |
| Runtime Service | ASTRAEON-X | Runs sessions, dispatch, lifecycle, and runtime health. | Runtime API, Event API. |
| Event Bus | ASTRAEON-X | Carries source-of-truth events. | Event Store, Logger, portals. |
| Logger | ASTRAEON-X | Records runtime diagnostics and validation evidence. | Persistence, reports. |
| Persistence | ASTRAEON-X | Stores event and log evidence. | Data Platform. |
| Validation Service | Laboratory | Executes validation recipes and produces PASS/FAIL. | Commander, Probe, Event Store. |
| Auto Release Pipeline | Developer | Verifies, commits, pushes, and checks remote sync. | Git, CI, AEP. |
| Probe Service | Hardware | Runs hardware observation and validation commands. | Hardware API, USB CDC, WiFi. |
| Commander Service | Commander | Coordinates operators, portals, automation, and sessions. | APIs, policy, event store. |
| Communication Engine | Communication | Routes events to LINE and future channels. | Communication API, user registry. |
| Business Engine | Business | Processes OCR, POS, accounting, inventory, and reports. | Business API, Business DB. |
| Knowledge Service | Knowledge | Stores facts, docs, lessons, and semantic index. | Knowledge API, AI. |
| Vision Service | Vision | Renders topology, wiring, state, and blockers. | Vision API, Digital Twin. |
| AI Diagnostics Service | AI | Produces grounded diagnostics and plans. | AI API, Knowledge, Event Store. |
| Identity Service | Security | Identifies users, devices, probes, services, and agents. | Security API, Policy Engine. |
| Policy Service | Security | Evaluates permissions and safety gates. | Kernel, Commander, Automation. |
| Telemetry Service | Data | Captures health, metrics, and time-series data. | Telemetry Store, dashboards. |
| Time Machine Service | Data | Replays historical events and state. | Event Store, Digital Twin. |
| Universe Graph Service | Data | Links domains, APIs, events, data, and dependencies. | Semantic Graph, Vision, AI. |

## Service Rules

- Services emit structured events.
- Services expose explicit APIs.
- Services write to a registry when they create durable resources.
- Services must declare dependencies and maturity level.
- Services must preserve ASTRAEON-X compatibility.
