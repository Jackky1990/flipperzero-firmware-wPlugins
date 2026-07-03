# ASTRAEON Integration Rules

## Required Questions

Every future feature must answer:

1. Which layer owns it?
2. Who owns it?
3. Which API does it expose?
4. Which events does it emit?
5. Which knowledge does it create or update?
6. Which portal or dashboard shows it?
7. Which mission owns the workflow?
8. Which permission or policy gates it?
9. Which registry records it?
10. Which documentation describes it?
11. Which compatibility boundary protects existing systems?
12. What is the preservation impact?

If any answer is missing, do not implement.

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
