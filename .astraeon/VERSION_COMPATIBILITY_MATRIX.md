# ASTRAEON Version Compatibility Matrix

## Semantic Versioning Proposal

Use semantic versioning per repository:

```text
MAJOR.MINOR.PATCH
```

- MAJOR: incompatible API, data, or governance changes.
- MINOR: backward-compatible capability additions.
- PATCH: fixes, clarifications, documentation updates, or internal changes.

Pre-release labels may be used:

```text
1.0.0-rc1
1.0.0-alpha.1
```

## Compatibility Policy

- `astraeon-universe` publishes architecture and interface compatibility.
- Implementation repositories declare the universe version they comply with.
- Breaking changes require migration notes and deprecation window.
- RogueMaster local `.astraeon/` remains valid until authority transfer is
  verified.

## Initial Compatibility Matrix

| Universe Version | astraeon-x | astraeon-hardware | astraeon-commander | astraeon-business | astraeon-communication | Policy |
| --- | --- | --- | --- | --- | --- | --- |
| `0.1.x` | RogueMaster `.astraeon/` source docs | Not created | Concepts only | Concepts only | Concepts only | Copy-first migration planning. |
| `0.2.x` | Cross-linked RogueMaster local state | Bootstrap P0 docs | Portal/CLI architecture | Business architecture | LINE gateway architecture | Universe repo becomes index source. |
| `1.0.x` | Embedded SDK v1 compatible | Probe P1 compatible | Commander API compatible | Business API draft compatible | Communication API draft compatible | Universe authoritative after approval. |

## Migration Policy

- Copy before move.
- Verify before authority transfer.
- Cross-link before reducing duplicate docs.
- Retire duplicates only after Architect approval.
- Maintain compatibility notes for each repository.

## Version Records

| Component | Current Planning Version | Notes |
| --- | --- | --- |
| `astraeon-universe` | `0.1.0-plan` | Bootstrap plan in RogueMaster only. |
| RogueMaster ASTRAEON-X docs | `v2.0` | Universe Blueprint v2.0 source docs. |
| Embedded SDK | `v1.0-rc1` | Software validated; hardware validation blockers remain. |
