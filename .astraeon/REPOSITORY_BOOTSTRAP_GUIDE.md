# ASTRAEON Universe Repository Bootstrap Guide

## Goal

Create the initial `astraeon-universe` repository foundation without moving or
deleting RogueMaster files.

## Bootstrap Commands

```sh
mkdir -p ASTRAEON/astraeon-universe
cd ASTRAEON/astraeon-universe
git init
mkdir -p governance architecture blueprints registries capabilities operations standards rfc adr references migration
touch README.md MASTER_INDEX.md MASTER_STATUS.md MASTER_ROADMAP.md MASTER_DEPENDENCY.md MASTER_DECISIONS.md
```

## README Template

```md
# ASTRAEON Universe

ASTRAEON Universe is the architecture, governance, registry, roadmap, and
compatibility source of truth for the ASTRAEON ecosystem.

Implementation repositories own implementation. This repository owns
cross-repository architecture and governance.
```

## Governance Copy Plan

Copy from RogueMaster `.astraeon/` into the new layout:

- Engineering contracts -> `governance/`
- Master charter and ecosystem architecture -> `architecture/`
- Universe blueprint files -> `blueprints/`
- Registry documents -> `registries/`
- Capability documents -> `capabilities/`
- Roadmap/status/handoff docs -> `operations/`
- Integration rules and principles -> `standards/`
- Migration plan and guide -> `migration/`

## First Commit Plan

Commit message:

```text
Bootstrap ASTRAEON Universe repository foundation
```

Commit contents:

- empty directory structure with placeholder docs
- README
- master index
- copied governance docs
- migration audit log

## Validation Checklist

- Repository initializes cleanly.
- Required directories exist.
- Master docs exist.
- Copied docs preserve source references.
- RogueMaster `.astraeon/` remains unchanged.
- No firmware, SDK, runtime, build, or release files are modified.
- Cross-links are not added until Phase M4.

## Future GitHub Remote Plan

After local bootstrap is verified:

```sh
git remote add origin git@github.com:ASTRAEON/astraeon-universe.git
git push -u origin main
```

Remote creation and push require Architect approval and working credentials.
