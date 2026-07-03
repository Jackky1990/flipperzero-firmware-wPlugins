# ASTRAEON Universe Link

## Repository

- Remote: `git@github.com:Jackky1990/astraeon-universe.git`
- Local path: `~/ASTRAEON-X/astraeon-universe`
- Branch: `main`

## Purpose

`astraeon-universe` is the candidate architecture, governance, registry,
roadmap, standards, RFC, ADR, and migration source of truth for the ASTRAEON
ecosystem.

RogueMaster remains the active Flipper reference target and Embedded SDK local
state repository.

## Current Authority Status

- `astraeon-universe`: candidate source of truth.
- RogueMaster `.astraeon/`: active local Embedded SDK and Flipper reference
  target state.
- No destructive migration has been approved.

## Migration Policy

Use the preservation sequence:

Preserve -> Verify -> Copy -> Cross-link -> Migrate -> Retire duplicate only
after Architect approval.

Documents may be copied from RogueMaster `.astraeon/` into
`astraeon-universe`, but RogueMaster local state files must remain active until
the migration is verified and the Architect approves authority transfer.

## Cross-Link Rules

- Do not delete RogueMaster `.astraeon/` files.
- Do not move RogueMaster `.astraeon/` files.
- Do not rewrite history.
- Do not treat `astraeon-universe` as authoritative until Architect approval.
- Keep RogueMaster `.astraeon/PROJECT_STATUS.json`,
  `.astraeon/NEXT_ACTION.md`, and `.astraeon/HANDOFF.md` current for
  Flipper/Embedded SDK work.
- Use `astraeon-universe` for ecosystem-level architecture review during the
  authority transfer period.

## Next Task

Perform ASTRAEON Universe authority transfer review.
