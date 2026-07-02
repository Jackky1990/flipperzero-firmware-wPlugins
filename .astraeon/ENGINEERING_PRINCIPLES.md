# ASTRAEON Engineering Principles

## Observe Before Control

Prefer passive observation before active control. Hardware behavior must be
measured and recorded before ASTRAEON drives pins, sends packets, or triggers
automation.

## Simulate Before Execute

Validation recipes and automation plans should be simulated before real hardware
execution. Simulation must check topology, resources, permissions, and expected
events.

## Safety Before Speed

Fast progress is not useful if it damages hardware or corrupts architecture.
Voltage safety, pin safety, permissions, and recovery paths come first.

## Events Are The Source Of Truth

Runtime behavior, hardware validation, probe captures, automation decisions,
and failures must produce events. Logs can explain events, but events define
what happened.

## Everything Is Versioned

Firmware, probe firmware, protocols, validation recipes, reports, pin maps,
hardware registry entries, and knowledge documents must be versioned.

## Knowledge Never Stops Growing

Datasheets, reference designs, validation history, known issues, repair notes,
and lessons learned are part of the system. Every failure should improve future
diagnostics.

## Hardware And AI Evolve Together

AI diagnostics must learn from real hardware behavior. Hardware validation must
produce structured evidence that AI can reason over.

## Infrastructure Before Features

Build the control plane, validation path, telemetry, logging, permissions, and
recovery foundation before adding broad feature surfaces.

## Reusable By Design

Drivers, probes, validation recipes, commander commands, event schemas, and SDK
contracts should be reusable across Flipper, ESP32, Ethernet nodes, robotics,
and future targets.

## Separate Vision From Execution

Vision can model topology, state, prediction, and recommended actions.
Execution requires Commander, Kernel, resource, permission, and validation
gates.

## Thin Adapters

Hardware adapters should expose minimal hardware-specific primitives. Workflows,
policy, logging, persistence, and validation belong above adapters.

## Controller Ownership

Controllers own sessions, sequence, retries, timeout, cancellation, logs,
events, and persistent evidence.

## No Hidden Hardware Side Effects

No startup write, render write, background polling, hidden pin mode change, or
implicit hardware control is acceptable without explicit approval.

## SAFE PATCH Workflow

Every future implementation must:

- Inspect repository first.
- Preserve behavior.
- Avoid direct firmware modification unless approved.
- Document architecture impact.
- Verify before commit.
- Use auto release pipeline when appropriate.
- Keep one logical change per commit.
- Stop and report conflicts.
