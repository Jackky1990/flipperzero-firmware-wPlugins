# ASTRAEON Maturity Model

## Levels

- L0 Concept: idea documented, no architecture commitment.
- L1 Architecture: design approved, interfaces and ownership understood.
- L2 Prototype: initial implementation or proof exists.
- L3 Software Validated: host/runtime/CI verification passes.
- L4 Hardware Validated: real hardware evidence accepted.
- L5 Production Ready: operationally stable, documented, monitored, recoverable.
- L6 Certified: compliance, stress, safety, and audit evidence accepted.
- L7 Autonomous: policy-gated autonomous operation with simulation, recovery,
  and learning.

## Initial Maturity Assignments

| Component | Level | Notes |
| --- | --- | --- |
| Runtime | L3 | Runtime tests and architecture audit pass. |
| GPIO | L3 | Software/controller path exists; controlled write hardware validation blocked by hardware. |
| UART | L3 | TX/RX software paths exist; hardware validation needs known-good 3.3V USB-UART setup. |
| Release Pipeline | L3 | Auto release verifies, commits, pushes, and confirms sync. |
| Hardware Lab | L1 | Architecture documented; separate project not yet created. |
| Business | L1 | Concepts documented; implementation not started in this repo. |
| Communication | L1 | LINE/event gateway concepts documented. |
| Commander | L1 | Portal/CLI architecture documented. |
| Vision | L1 | Topology and graph architecture documented. |
| Knowledge | L1 | Knowledge scope documented. |
| AI | L1 | AI role and boundaries documented. |
| Infrastructure | L1 | Node01/Docker/MQTT/Grafana concepts documented. |
| Media | L0 | Scope identified. |
| Home | L0 | Scope identified. |
| Robotics | L0 | Future scope identified. |

## Advancement Rules

- L1 requires architecture approval.
- L2 requires scoped implementation and tests.
- L3 requires CI and runtime verification.
- L4 requires real hardware evidence.
- L5 requires monitoring, recovery, docs, and release readiness.
- L6 requires stress, certification, and audit evidence.
- L7 requires simulation, policy, telemetry, recovery, and autonomous learning.
