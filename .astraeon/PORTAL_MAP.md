# ASTRAEON Portal Map

## Portal Layer

The ASTRAEON-X Portal Layer evolves existing dashboard concepts into
domain-specific operator surfaces. Current example to preserve:

- `http://100.78.191.24:8504/executive`

## Portals

| Portal | Purpose | Primary Data | Key Actions |
| --- | --- | --- | --- |
| Executive | High-level health, roadmap, finance, validation, blockers. | Status, business, validation, risks. | Review, approve, prioritize. |
| Operations | Daily operations, tasks, notifications, summaries. | Business, communication, automation. | Dispatch, acknowledge, close. |
| Engineering | Runtime, SDK, CI, releases, architecture state. | Git, CI, AEO, events. | Inspect, release, handoff. |
| Hardware | Probe nodes, wiring, pin maps, validation sessions. | Hardware registry, events, telemetry. | Run approved recipes, inspect evidence. |
| Business | POS, OCR, inventory, accounting, profit/loss. | Business DB, accounting DB. | Review reports, correct records. |
| Vision | Topology, wiring, graphs, blockers, prediction. | Vision graph, Digital Twin. | Inspect graph, mark blockers. |
| Knowledge | Datasheets, protocols, repair notes, lessons. | Knowledge DB, semantic index. | Search, cite, link evidence. |
| AI | Diagnoses, plans, agents, memory, tool runs. | AI Memory, events, knowledge. | Review recommendations, approve actions. |
| Laboratory | Recipes, fixtures, certifications, stress tests. | Validation store, hardware registry. | Simulate, execute approved tests. |
| Media | Capture sessions, streams, video/image evidence. | Media DB, event references. | Review, tag, attach evidence. |
| Home | Sensors, environment, Home Assistant state. | Telemetry, home events. | Monitor, approve automation. |
| Automation | Workflows, schedules, recovery runs. | Mission store, automation events. | Dry-run, execute, cancel. |
| Settings | Identity, policy, registries, connectors. | Security, registries. | Configure, grant, revoke. |

## Portal Rules

- Portals display state; controllers execute actions.
- Every portal action must map to API, event, permission, and registry updates.
- Portals must show evidence for hardware and business decisions.
- Portal state must be reproducible from data stores and events.
