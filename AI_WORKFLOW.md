# ASTRAEON AI Engineering Workflow

ASTRAEON development follows a contract-first workflow. Architecture decisions come from the Architect, implementation comes from Codex, verification comes from CI, and accepted state lives in Git.

## Workflow

1. Inspect the repository and read the relevant AEO files.
2. Confirm the active task, scope, and architecture constraints.
3. Implement one logical change only.
4. Avoid firmware, runtime, SDK, demo app, build, or release changes unless the task explicitly allows them.
5. Run the required CI/AEP verification.
6. Commit only after green verification.
7. Push the committed change to origin.
8. Report files changed, verification result, commit hash, and the remaining next task.

## Conflict Handling

If implementation conflicts with architecture documents, engineering contracts, or the current task scope, Codex must stop and explain the issue instead of redesigning the system independently.

## Current Source of Truth

- `.astraeon/contracts/engineering_contract.yaml`
- `.astraeon/PROJECT_STATUS.json`
- `.astraeon/CURRENT_TASK.md`
- `.astraeon/NEXT_ACTION.md`
- Git history and current branch state
