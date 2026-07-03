# ASTRAEON Service Registry

## Known And Planned Services

| Service | Owner | Layer | Dependencies | Status | Evolution |
| --- | --- | --- | --- | --- | --- |
| FastAPI | Infrastructure | API Gateway | Python, service modules, data stores | Preserved concept | Domain APIs and internal service endpoints. |
| Streamlit | Commander | Portal | Python, FastAPI, data services | Preserved concept | Portal registry and dashboard standards. |
| Grafana | Infrastructure | Telemetry | Prometheus, metrics exporters | Preserved concept | Unified operations and engineering dashboards. |
| Prometheus | Infrastructure | Telemetry | exporters, services | Preserved concept | Runtime, Node01, Probe, service metrics. |
| Redis | Infrastructure | Data Platform | services, queues | Planned | Cache, queue, session coordination. |
| MQTT | Communication | Event Gateway | broker, Node01, devices | Preserved concept | Device and notification event bridge. |
| Docker | Infrastructure | Deployment | Node01, service images | Preserved concept | Reproducible service deployment. |
| Home Assistant | Home | Automation | MQTT, ESP32, sensors | Preserved concept | Home telemetry and automations. |
| Ollama | AI | Local AI | Node01/GPU/CPU resources | Planned | Local inference and offline AI diagnostics. |
| MinIO | Data | Object Storage | Docker, storage volumes | Planned | Media, reports, validation evidence, backups. |
| Node01 | Infrastructure | Host | Docker, MQTT, Grafana, storage | Preserved concept | Core lab server and service host. |
| Weather | Cloud | Context Service | weather API, location layer | Planned | Weather-aware automation and reporting. |
| LINE | Communication | Event Gateway | LINE Messaging API, user registry | Preserved concept | Notifications, commands, replies, broadcasts. |
| OpenAI | Cloud/AI | Cloud AI | API keys, policy, logs | Planned | Reasoning, planning, summarization, diagnostics. |
| Backup | Infrastructure | Recovery | storage, cloud, schedules | Planned | Versioned backups and restore reports. |

## Service Rules

- Every service must declare owner, API, events, data stores, health checks, and
  policy gates.
- Services that touch secrets must integrate with Security before production.
- Services must not bypass ASTRAEON-X event and registry boundaries.
