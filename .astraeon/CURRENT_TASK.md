# Current Task

## Name

Continue Release Framework Refactor by implementing artifact helpers.

## Intent

Move release artifact knowledge out of ad hoc script logic and into focused helper modules so the release path stays small, reviewable, and testable.

## Acceptance Criteria

- Artifact helpers expose release artifact metadata from one place.
- Release pipeline uses helpers instead of duplicating artifact paths.
- Existing behavior is preserved.
- AEP/CI verification passes.
- Change is committed only after green verification.
