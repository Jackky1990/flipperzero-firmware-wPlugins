# Next Action

R6R-3 is complete. The release framework now routes through `pipeline.py`, while `release.py` remains a thin orchestration entry point.

## Architecture Status

Approved. The current release flow is:

- `release.py`
- `release/pipeline.py`
- `release/artifact.py`
- `release/manifest.py` (next)
- `release/notes.py` (future)
- `release/github.py` (future)

## Remaining Work

- Implement the R6R-4 Manifest Layer.
- Keep manifest generation isolated in `release/manifest.py`.
- Preserve current release behavior until the Architect approves the next integration step.

## Next Implementation Step

Implement manifest helpers for release artifact metadata without adding release notes or GitHub release behavior.
