# ASTRAEON Integration Rules

## Required Questions

Every future feature must answer:

1. Which layer owns it?
2. Which event does it emit?
3. Which API does it expose?
4. Which registry records it?
5. Which knowledge does it create or update?
6. Which dashboard or portal shows it?
7. Which permission or policy gates it?
8. How does it preserve existing ASTRAEON-X systems?

## Preservation Rule

Preserve -> Verify -> Enhance -> Integrate.

Do not rewrite ASTRAEON-X systems without:

- proven benefit
- compatibility plan
- migration plan
- verification strategy
- rollback strategy
- Architect approval

## Execution Boundaries

- Vision and Knowledge do not execute hardware actions.
- AI recommends and explains; Kernel and Commander gate execution.
- Hardware adapters remain thin.
- Controllers own workflow.
- Events are the source of truth.
- Registries own durable identity and capability records.
- Policy gates all control paths.
- Simulation precedes execution where hardware or operations risk exists.

## SAFE PATCH Rule

Implementation must remain small, reviewable, and verifiable:

- inspect repository first
- one logical change
- no broad rewrite
- no unapproved firmware-core modification
- verify before commit
- auto release only after green verification
