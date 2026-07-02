# ASTRAEON Project Registry

## Current Repository

| Field | Value |
| --- | --- |
| Repository | RogueMaster |
| Role | Flipper reference target under ASTRAEON-X Embedded SDK work |
| Branch | astraeon/release-bundle-v1 |
| Core Assets | Runtime, SDK, GPIO, UART, AEP, CI, SAFE PATCH, Auto Release |
| Boundary | Do not mix Probe firmware or non-Flipper ecosystem projects into RogueMaster |

## Future Repositories

| Repository | Purpose | Initial Owner Domain | Relationship To ASTRAEON-X |
| --- | --- | --- | --- |
| `astraeon-x` | Core platform, embedded SDK, contracts, reference targets. | ASTRAEON-X | Core platform. |
| `astraeon-governance` | Contracts, approvals, policy, roadmaps. | Governance | Defines execution rules. |
| `astraeon-hardware` | Probe firmware, validation board, hardware lab. | Hardware/Laboratory | Validates ASTRAEON-X targets externally. |
| `astraeon-commander` | CLI, portals, dashboards, orchestration. | Commander | Operates through APIs/events. |
| `astraeon-business` | POS, OCR, accounting, inventory. | Business | Consumes events and data platform. |
| `astraeon-communication` | LINE, notifications, messaging. | Communication | Event gateway to users. |
| `astraeon-knowledge` | Datasheets, protocols, lessons, semantic search. | Knowledge | Grounds AI and operators. |
| `astraeon-vision` | Topology, digital twin visuals, dashboards. | Vision | Displays hardware and system state. |
| `astraeon-ai` | Agents, diagnostics, planning, memory. | AI | Recommends actions through policy gates. |
| `astraeon-infrastructure` | Node01, Docker, MQTT, Grafana, storage. | Infrastructure | Runs shared services. |
| `astraeon-media` | FX3, DJI, streaming, content pipeline. | Media | Adds media evidence and workflows. |
| `astraeon-home` | Home Assistant, ESP32, sensors. | Home | Extends environment automation. |
| `astraeon-robotics` | Drones, PLC, motors, robotic systems. | Robotics | Future physical execution domain. |
| `astraeon-laboratory` | Validation recipes, certification, stress tests. | Laboratory | Produces hardware evidence. |
| `astraeon-cloud` | OpenAI, Ollama, weather, maps, backup. | Cloud | Provides external services. |
| `astraeon-developer` | SDK, CLI, templates, plugin tooling. | Developer | Improves developer experience. |
| `astraeon-docs` | Public and internal documentation. | Governance/Knowledge | Stable documentation hub. |

## Registry Rules

- Each repository must declare owning domain and integration boundary.
- Shared APIs must be versioned.
- Cross-repo changes require compatibility review.
- RogueMaster stays a Flipper reference target unless Architect changes scope.
