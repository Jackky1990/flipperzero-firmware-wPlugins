# ASTRAEON Interface Contract Catalog

| Contract | Owner | Event Domains | Data Contracts | Current Status | Future Repository |
| --- | --- | --- | --- | --- | --- |
| Runtime API | ASTRAEON-X | `runtime.*` | Runtime state, health, diagnostics, sessions. | Software validated in RogueMaster. | `astraeon-x` |
| Event API | ASTRAEON-X/Data | all domains | Event envelope, timestamp, source, session id, status. | Software validated locally; universe-level contract planned. | `astraeon-x`, `astraeon-universe` |
| Device API | ASTRAEON-X/Hardware | `device.*` | Device capability, resource, status, diagnostics. | GPIO/UART foundations exist. | `astraeon-x`, `astraeon-hardware` |
| Hardware API | Hardware/Laboratory | `hardware.*`, `validation.*` | Probe commands, pin/bus descriptors, validation reports. | Architecture/planning. | `astraeon-hardware`, `astraeon-laboratory` |
| Business API | Business | `business.*` | POS, OCR, accounting, inventory, cashflow records. | Architecture/planning. | `astraeon-business` |
| Communication API | Communication | `communication.*` | User registry, message payload, delivery status. | Architecture/planning. | `astraeon-communication` |
| Commander API | Commander | `commander.*`, `automation.*` | Mission commands, portal actions, approvals. | Architecture/planning. | `astraeon-commander` |
| Knowledge API | Knowledge | `knowledge.*` | Documents, citations, lessons, semantic index records. | Architecture/planning. | `astraeon-knowledge` |
| Vision API | Vision | `vision.*`, `digitaltwin.*` | Graph nodes, edges, state snapshots, blockers. | Architecture/planning. | `astraeon-vision` |
| AI API | AI | `ai.*` | Prompt, tool call, diagnosis, plan, evidence references. | Architecture/planning. | `astraeon-ai` |
| Automation API | Automation/Commander | `automation.*` | Workflow, dry-run, execution, recovery, result. | Architecture/planning. | `astraeon-commander` |
| Security API | Security | `security.*` | Identity, policy decision, audit, secret reference. | Architecture/planning. | `astraeon-universe`, future security implementation repo |

## Contract Rules

- Contract definitions live in `astraeon-universe` after migration.
- Implementation repositories own code and tests for their contracts.
- Breaking contract changes require version update, migration guide, and
  compatibility review.
