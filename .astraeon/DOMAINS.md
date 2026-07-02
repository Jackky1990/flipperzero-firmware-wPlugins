# ASTRAEON Domains

## Kernel

Owns event, device, session, resource, permission, driver, scheduler, plugin,
logger, telemetry, diagnostics, recovery, and API gateway primitives.

## Core

Owns shared services, lifecycle, configuration, command routing, system health,
and cross-domain contracts.

## AI

Owns reasoning, diagnosis, planning, explanation, anomaly detection, and
operator assistance. AI recommends and explains; execution still passes through
Kernel and Commander gates.

## Hardware

Owns Probe nodes, hardware drivers, validation recipes, board definitions,
wiring maps, logic capture, telemetry, and hardware registry.

## Vision

Owns topology, connections, signals, state, history, prediction, wiring map,
validation status, hardware blockers, and dashboards.

## Knowledge

Owns datasheets, reference designs, protocols, pin maps, validation history,
repair notes, known issues, and lessons learned.

## Automation

Owns repeatable workflows, scheduled validation, infrastructure actions,
operator-approved recipes, and recovery automation.

## Network

Owns Node01, MQTT, TCP, HTTP, USB CDC routing, probe networking, edge node
connectivity, and remote lab transport.

## Security

Owns identity, permissions, policy, audit, secret handling, device trust,
command authorization, and remote access boundaries.

## Robotics

Owns robotic controllers, sensors, actuators, motion validation, safety gates,
simulation, telemetry, and recovery.

## Labs

Owns physical lab setup, test fixtures, validation boards, hardware inventory,
bench procedures, and lab reports.

## SDK

Owns portable contracts, thin platform adapters, host tests, runtime tests,
public API stability, and target integration guidelines.

## Plugins

Owns optional capabilities loaded through explicit contracts. Plugins must
declare resources, permissions, events, and failure behavior.

## Domain Rules

- Domains communicate through events, sessions, and contracts.
- Hardware adapters remain thin.
- Controllers own workflow.
- Knowledge and Vision do not directly execute hardware actions.
- Automation requires permissions and observable events.
- Security gates all control paths.
