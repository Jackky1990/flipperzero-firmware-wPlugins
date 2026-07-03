# ASTRAEON Dependency Graph

## Graph

```text
Governance
  -> Engineering Contract
  -> SAFE PATCH
  -> CI / AEP

ASTRAEON-X Runtime
  -> Session Engine
  -> Event Bus
  -> Logger
  -> Persistence
  -> SDK
  -> GPIO / UART

SDK
  -> Thin Adapters
  -> Device HAL
  -> Host/Runtime Tests

Hardware
  -> ASTRAEON-X SDK
  -> Probe
  -> Hardware Registry
  -> Validation Engine

Commander
  -> Runtime / Event APIs
  -> Portals
  -> Mission Registry
  -> Policy Engine

Business
  -> Data Platform
  -> Communication
  -> Commander
  -> AI

Communication
  -> Event Bus
  -> User Registry
  -> LINE / MQTT / Future Channels

Vision
  -> Device Registry
  -> Hardware Registry
  -> Digital Twin
  -> Knowledge

Knowledge
  -> Data Platform
  -> Semantic Search
  -> AI

AI
  -> Knowledge
  -> Event Store
  -> Vision
  -> Policy Engine

Infrastructure
  -> Node01
  -> Docker
  -> Grafana / Prometheus / MQTT / Storage

Media
  -> Storage
  -> Device Registry
  -> Mission Registry

Home
  -> Home Assistant
  -> MQTT
  -> Telemetry
```

## Dependency Rules

- Dependencies flow through APIs, events, registries, and policy.
- No domain may directly mutate another domain's protected state.
- RogueMaster remains a Flipper reference target, not the full universe.
