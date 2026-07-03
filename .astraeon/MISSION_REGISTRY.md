# ASTRAEON Mission Registry

## Rule

Everything becomes Missions. A mission binds goal, owner, session, resources,
events, evidence, policy, and result.

## Mission Types

| Mission | Layer | Owner | API | Events | Portal | Policy |
| --- | --- | --- | --- | --- | --- | --- |
| GPIO Validation | Laboratory | Hardware | Hardware API | `validation.*`, `hardware.gpio.*` | Hardware/Laboratory | Hardware safety policy |
| UART Validation | Laboratory | Hardware | Hardware API | `validation.*`, `hardware.uart.*` | Hardware/Laboratory | Hardware safety policy |
| Slip OCR | Business | Business | Business API | `business.ocr.*` | Business | Business data policy |
| Daily Accounting | Business | Business | Business API | `business.accounting.*` | Executive/Business | Accounting policy |
| Backup | Infrastructure | Infrastructure | Infrastructure API | `infrastructure.backup.*` | Operations/Settings | Data retention policy |
| Deploy | Developer | Developer | Developer API | `developer.release.*` | Engineering | Release policy |
| Drone Flight | Robotics/Media | Robotics | Robotics API | `robotics.flight.*`, `media.capture.*` | Media/Robotics | Flight safety policy |
| Media Import | Media | Media | Media API | `media.import.*` | Media | Media evidence policy |
| Knowledge Sync | Knowledge | Knowledge | Knowledge API | `knowledge.sync.*` | Knowledge | Source/citation policy |
| Hardware Scan | Hardware | Hardware | Hardware API | `hardware.scan.*` | Hardware | Resource policy |
| Node Health | Infrastructure | Infrastructure | Infrastructure API | `infrastructure.health.*` | Operations | Monitoring policy |
| AI Summary | AI | AI | AI API | `ai.summary.*` | AI/Executive | AI output policy |

## Mission Record Requirements

- mission id
- owner
- layer
- resources
- policy decision
- session id
- emitted events
- evidence references
- result
- rollback or recovery note
