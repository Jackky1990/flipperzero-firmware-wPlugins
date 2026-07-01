# ASTRAEON Engineering Orchestrator

The ASTRAEON Engineering Orchestrator (AEO) keeps project state, current tasks, implementation evidence, review notes, and engineering memory close to the repository.

## Roles

- ChatGPT is the Architect: defines direction, decomposes work, reviews architecture, and keeps the system-level plan coherent.
- Codex is the Engineer: inspects the repository, makes scoped patches, runs verification, and reports implementation evidence.
- CI is QA: runs repeatable verification and blocks regressions before changes become release candidates.
- Git is the source of truth: accepted project state, code, reports, and decisions are committed and pushed.

## Operating Model

AEO favors one logical change at a time. Every change starts with repository inspection, preserves existing behavior, runs CI/AEP verification, and records the result before commit.

## Directory Map

- `CURRENT_TASK.md`: active task intent and acceptance criteria.
- `IMPLEMENTATION_REPORT.md`: implementation evidence template.
- `REVIEW_REPORT.md`: architecture and code review template.
- `NEXT_ACTION.md`: next engineering action.
- `PROJECT_STATUS.json`: machine-readable project phase and health.
- `contracts/engineering_contract.yaml`: working rules for engineering execution.
- `memory/`: durable decisions, lessons, and mistakes.
