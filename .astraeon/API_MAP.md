# ASTRAEON API Map

## API Families

| API | Purpose | Initial Operations | Event Domains |
| --- | --- | --- | --- |
| Runtime API | Manage runtime lifecycle, sessions, health, and diagnostics. | open, close, status, diagnostics. | `runtime.*` |
| Event API | Publish, query, correlate, and replay events. | emit, subscribe, query, replay. | all domains |
| Device API | Represent devices, capabilities, adapters, and sessions. | enumerate, acquire, release, status. | `device.*` |
| Hardware API | Control probe and hardware validation operations. | probe, capture, validate, report. | `hardware.*`, `validation.*` |
| Business API | Process business records and operations. | ingest OCR, sale, purchase, inventory, report. | `business.*` |
| Communication API | Send messages and route notifications. | reply, push, broadcast, register user. | `communication.*` |
| Commander API | Run operator workflows and portals. | start mission, run recipe, inspect, approve. | `commander.*` |
| Knowledge API | Store and retrieve engineering knowledge. | ingest, link, search, cite. | `knowledge.*` |
| Vision API | Query topology, state, wiring, and blockers. | render graph, update state, show blockers. | `vision.*` |
| AI API | Request reasoning, planning, diagnostics, and summaries. | diagnose, plan, explain, optimize. | `ai.*` |
| Automation API | Execute approved workflows. | dry-run, execute, cancel, recover. | `automation.*` |
| Security API | Manage identity, secrets, policy, and audit. | authorize, audit, grant, revoke. | `security.*` |
| Data API | Store and query events, telemetry, registries, and memory. | write, query, replay, snapshot. | `data.*` |
| DigitalTwin API | Maintain live and historical modeled state. | update, simulate, compare, snapshot. | `digitaltwin.*` |
| Developer API | Build plugins, templates, SDKs, and compatibility checks. | scaffold, verify, release, test. | `developer.*` |

## API Rules

- APIs must be versioned.
- APIs must map errors to a documented error model.
- APIs that control hardware must require session, resource, and policy gates.
- APIs must emit events for state changes.
- APIs should be portable where practical.
