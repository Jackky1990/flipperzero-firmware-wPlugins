# ASTRAEON Cross-Repository Governance

## Source Of Truth Rule

Each repository owns implementation for its domain.

`astraeon-universe` owns architecture, governance, indexes, registries,
compatibility policy, migration records, and cross-repository decisions.

## Repository Rules

### `astraeon-x`

- Owns core platform implementation.
- Consumes universe contracts and standards.
- Must report architecture impact when changing public APIs or runtime behavior.

### `astraeon-hardware`

- Owns Probe firmware, hardware lab tools, validation board docs, and hardware
  recipes.
- Must preserve ASTRAEON-X reference targets through documented interfaces.

### `astraeon-commander`

- Owns CLI, portals, dashboards, mission orchestration.
- Must use APIs and policy gates instead of direct cross-domain mutation.

### `astraeon-business`

- Owns POS, OCR, accounting, inventory, analytics.
- Must preserve raw evidence and audit trails.

### `astraeon-communication`

- Owns LINE, notification router, webhooks, messaging channels.
- Must protect secrets and user registry data.

### `astraeon-ai`

- Owns agents, diagnostics, planning, model/tool integration.
- Must cite evidence and remain policy-gated.

### `astraeon-knowledge`

- Owns datasheets, protocols, lessons, semantic search.
- Must separate verified facts from assumptions.

### `astraeon-vision`

- Owns topology, graph, digital twin visualization.
- Must not execute hardware actions directly.

### `astraeon-infrastructure`

- Owns Node01, Docker, MQTT, Grafana, Prometheus, storage, backup.
- Must expose service health and deployment state.

### `astraeon-media`

- Owns FX3, DJI, streaming, media evidence.
- Must link media to events, missions, and timestamps.

### `astraeon-home`

- Owns Home Assistant, ESP32 sensors, environment automation.
- Must require policy approval for control automation.

### `astraeon-laboratory`

- Owns validation recipes, stress tests, certification reports.
- Must simulate before hardware execution when risk exists.

## Change Governance

Cross-repository changes require:

1. Universe issue or RFC.
2. Owner repository approval.
3. Interface contract review.
4. Compatibility impact statement.
5. Verification plan.
6. Migration or rollback plan.

## Forbidden Governance Patterns

- One repository silently redefining another repository's architecture.
- Direct mutation of another domain's data or resources.
- Copying implementation code into `astraeon-universe`.
- Deleting migrated source docs before verification and approval.
