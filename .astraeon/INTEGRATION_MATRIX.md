# ASTRAEON Integration Matrix

| System | Layer | API | Events | Knowledge | Portal | Dashboard | AI | Dependencies |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Runtime | Kernel | Runtime API | `runtime.*` | Runtime docs | Engineering | Runtime health | Diagnostics | SDK, logger |
| SDK | Developer | Developer API | `developer.*` | SDK docs | Engineering | API status | Code review | Runtime, CI |
| GPIO | Hardware | Device/Hardware API | `device.*`, `hardware.gpio.*` | Pin maps | Hardware | Validation | Diagnostics | SDK, Probe |
| UART | Hardware | Device/Hardware API | `device.*`, `hardware.uart.*` | Protocol notes | Hardware | Validation | Diagnostics | SDK, Probe |
| Business | Business | Business API | `business.*` | Business rules | Business | P/L, cashflow | AI assistant | OCR, DB |
| Communication | Communication | Communication API | `communication.*` | Channel docs | Operations | Delivery status | Message drafting | LINE, MQTT |
| Hardware Lab | Laboratory | Hardware API | `validation.*` | Validation history | Laboratory | PASS/FAIL | Root cause | Probe, registry |
| Commander | Commander | Commander API | `commander.*` | Operator guides | All portals | Missions | Planning | APIs, policy |
| Vision | Vision | Vision API | `vision.*` | Graph docs | Vision | Topology | Prediction | data, twin |
| Knowledge | Knowledge | Knowledge API | `knowledge.*` | Knowledge DB | Knowledge | Search status | Grounding | docs, events |
| AI | AI | AI API | `ai.*` | Memory Fabric | AI | Agent status | Core | Knowledge, policy |
| Infrastructure | Infrastructure | Infrastructure API | `infrastructure.*` | Runbooks | Operations | Node health | Ops summary | Node01, Docker |
| Media | Media | Media API | `media.*` | Media notes | Media | Capture status | Editing | storage, devices |
| Home | Home | Home API | `home.*` | Home notes | Home | Environment | Automation advice | HA, MQTT |

## Matrix Rule

If a future feature cannot fill layer, owner, API, events, knowledge, portal,
mission, policy, registry, documentation, compatibility, and preservation
impact, do not implement it.
