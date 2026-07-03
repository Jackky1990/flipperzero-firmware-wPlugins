# ASTRAEON Universe Repository Plan

## Objective

Prepare migration of RogueMaster `.astraeon/` ecosystem documentation into a
new independent repository:

```text
ASTRAEON/astraeon-universe
```

This is a planning step only. Do not move, delete, or rewrite files in
RogueMaster during this phase.

## Repository Roles

- `astraeon-universe` becomes the single source of truth for ecosystem
  architecture, governance, registries, standards, roadmap, decisions,
  compatibility, and cross-repository indexes.
- RogueMaster remains the Flipper reference target under ASTRAEON-X Embedded
  SDK work.
- ASTRAEON-X remains the core platform.
- `astraeon-hardware` becomes the separate Probe and Hardware Lab repository.
- Other ASTRAEON repositories own implementation for their domain.

## Migration Rule

Preserve -> Verify -> Copy -> Cross-link -> Migrate -> Retire duplicate only
after approval.

## Current Source

```text
RogueMaster/.astraeon/
```

Current `.astraeon/` remains active until migration completeness, links,
compatibility, and governance are verified.

## Target Structure

```text
astraeon-universe/
|-- README.md
|-- MASTER_INDEX.md
|-- MASTER_STATUS.md
|-- MASTER_ROADMAP.md
|-- MASTER_DEPENDENCY.md
|-- MASTER_DECISIONS.md
|-- governance/
|-- architecture/
|-- blueprints/
|-- registries/
|-- capabilities/
|-- operations/
|-- standards/
|-- rfc/
|-- adr/
|-- references/
`-- migration/
```

## Initial Placement Plan

| Source Category | Target Directory | Notes |
| --- | --- | --- |
| Engineering contracts and rules | `governance/` | Keep AEO and SAFE PATCH rules traceable. |
| Master charter and ecosystem architecture | `architecture/` | Universe-level source of truth. |
| Universe blueprints | `blueprints/` | Preserve v1.0 and v2.0 history. |
| Registries | `registries/` | Project, device, service, portal, mission, preservation. |
| Capability documents | `capabilities/` | AI, business, communication, hardware, vision, knowledge, media, home. |
| Roadmaps and next actions | `operations/` | Master-level roadmap and current status. |
| Principles and integration rules | `standards/` | Governance checklist and engineering standards. |
| Migration documents | `migration/` | Audit, copy logs, verification reports. |
| Decisions | `adr/` or `MASTER_DECISIONS.md` | Architecture decision record index. |
| References | `references/` | External docs, source links, evidence indexes. |

## Non-Goals

- No firmware modification.
- No SDK behavior modification.
- No runtime behavior modification.
- No build-system or release-script modification.
- No destructive migration.
- No RogueMaster history rewrite.
