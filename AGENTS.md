# ASTRAEON Engineering Agents

This repository uses the ASTRAEON Engineering Orchestrator (AEO) to keep architecture, implementation, verification, and source control responsibilities explicit.

## ChatGPT (Architect)

- Owns architecture direction and engineering intent.
- Breaks work into scoped implementation tasks.
- Reviews architectural fit before changes become accepted project state.
- Decides when a module should be refactored into a clearer framework.
- Resolves architecture conflicts when implementation details do not match the contract or design.

## Codex (Implementation Engineer)

- Inspects the repository before modifying files.
- Implements one logical change at a time.
- Generates scoped patches that follow the architecture and engineering contract.
- Stops and reports conflicts instead of redesigning architecture independently.
- Runs verification and reports implementation evidence.

## CI (Verification)

- Runs repeatable checks for build, runtime, contract, and architecture safety.
- Blocks unverified changes from becoming release candidates.
- Confirms that firmware behavior, runtime contracts, and demo builds remain healthy.
- Provides the final green signal before commit and push.

## Git (Source of Truth)

- Stores accepted code, project state, contracts, decisions, and reports.
- Preserves the history of engineering decisions and implementation changes.
- Defines the current repository state through committed and pushed changes.
- Keeps AEO state auditable across Architect, Engineer, and CI workflows.
