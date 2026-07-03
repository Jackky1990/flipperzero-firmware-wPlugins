# ASTRAEON Communication Capability

| Capability | Status | Dependencies | Next Milestone |
| --- | --- | --- | --- |
| LINE Bot | Planned | LINE Messaging API, user registry | LINE Event Gateway design. |
| Messaging API | Planned | credentials, webhook, policy | API boundary and secrets model. |
| Reply | Planned | webhook, session context | Reply command contract. |
| Push | Planned | user registry, policy | Push notification contract. |
| Broadcast | Planned | audience policy, audit | Broadcast safety policy. |
| Webhook | Planned | FastAPI, identity, events | Webhook receiver. |
| MQTT | Preserved concept | broker, Node01, devices | MQTT event notification bridge. |
| Telegram | Concept | bot API, user registry | Future channel design. |
| Email | Concept | SMTP/service provider | Future notification fallback. |
| Voice | Concept | speech service, policy | Future voice interface. |

## Communication Rule

Event -> Communication Engine -> Channel -> User.

No communication channel may bypass user registry, audit, or policy gates.
