# ASTRAEON Universe Migration Guide

## Migration Policy

Migration is copy-first and verification-gated. RogueMaster `.astraeon/`
remains active until Architect approves authority transfer.

## Phases

### Phase M0: Audit Current `.astraeon/`

Actions:

- List every `.astraeon/` file.
- Classify each file by target directory.
- Identify RogueMaster-local state that should remain local.
- Identify ecosystem-wide docs that should be copied.
- Produce an audit report under `migration/`.

Exit criteria:

- Every source file has a target classification.
- No ambiguous ownership remains.

### Phase M1: Create `astraeon-universe` Repository

Actions:

- Create the new repository locally.
- Add bootstrap layout.
- Add README and master index.
- Keep repository independent from RogueMaster.

Exit criteria:

- New repository exists.
- Initial layout matches the repository plan.

### Phase M2: Copy Docs Into Organized Structure

Actions:

- Copy, do not move, selected `.astraeon/` docs.
- Preserve filenames where practical.
- Record source path and target path in migration log.

Exit criteria:

- Copied docs are present in the target structure.
- RogueMaster `.astraeon/` remains unchanged.

### Phase M3: Verify Completeness

Actions:

- Compare source inventory against copied inventory.
- Verify required master docs exist.
- Verify links and indexes.
- Run documentation consistency checks.

Exit criteria:

- Completeness report passes.
- Missing or intentionally local files are documented.

### Phase M4: Add Cross-Links From RogueMaster

Actions:

- Add RogueMaster `.astraeon/` links to `astraeon-universe`.
- Mark which documents are authoritative in the universe repo.
- Keep local Embedded SDK state in RogueMaster.

Exit criteria:

- Readers can navigate from RogueMaster to the universe repo.
- No local state is lost.

### Phase M5: Reduce RogueMaster `.astraeon/`

Actions:

- Keep Flipper/Embedded SDK local state only.
- Retain local handoff, current task, project status, and contract references.
- Do not delete duplicates until verification and approval.

Exit criteria:

- RogueMaster `.astraeon/` scope is local and explicit.

### Phase M6: Make `astraeon-universe` Authoritative

Actions:

- Architect approves authority transfer.
- Update cross-repository governance.
- Mark `astraeon-universe` as architecture/governance/index source of truth.

Exit criteria:

- No destructive changes pending.
- All repository owners understand source-of-truth boundaries.

## Destructive Actions Forbidden Until Approval

- Deleting RogueMaster `.astraeon/` docs.
- Rewriting Git history.
- Renaming source docs without migration log.
- Replacing local project state with universe-level state.
- Broad rewrite of architecture documents.
