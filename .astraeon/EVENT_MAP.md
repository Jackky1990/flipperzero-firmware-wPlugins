# ASTRAEON Event Map

## Event Domains

| Domain | Purpose | Example Events |
| --- | --- | --- |
| `runtime.*` | Runtime lifecycle and health. | `runtime.session.opened`, `runtime.health.updated` |
| `device.*` | Device capabilities and adapter state. | `device.capability.detected`, `device.resource.released` |
| `hardware.*` | Probe and bus operations. | `hardware.uart.tx`, `hardware.gpio.edge` |
| `business.*` | Business operations and records. | `business.sale.recorded`, `business.inventory.updated` |
| `communication.*` | Messages and notification delivery. | `communication.line.push.sent` |
| `infrastructure.*` | Node01, Docker, MQTT, storage, service health. | `infrastructure.service.started` |
| `media.*` | Capture, streaming, and media evidence. | `media.capture.completed` |
| `home.*` | Home sensors and automation. | `home.sensor.updated` |
| `validation.*` | Recipes, sessions, PASS/FAIL reports. | `validation.recipe.passed` |
| `security.*` | Identity, policy, audit, secrets. | `security.policy.denied` |
| `ai.*` | Reasoning, diagnostics, plans, agent actions. | `ai.diagnosis.generated` |
| `knowledge.*` | Knowledge ingestion and linking. | `knowledge.datasheet.indexed` |
| `vision.*` | Topology, graph, dashboard state. | `vision.blocker.updated` |
| `automation.*` | Workflow execution and recovery. | `automation.workflow.completed` |
| `data.*` | Store, replay, snapshot, and retention events. | `data.snapshot.created` |
| `digitaltwin.*` | Modeled live state and simulation. | `digitaltwin.state.updated` |
| `developer.*` | SDK, plugin, CI, release events. | `developer.release.synced` |

## Event Rules

- Events are the source of truth.
- Every event should carry timestamp, source, session id when applicable, status,
  and correlation id.
- Hardware and automation events must identify resource and policy decision.
- Business events must preserve evidence references.
- AI events must distinguish recommendation from execution.
