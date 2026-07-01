# Next Action

R6R-5 is complete. The release framework now includes a GitHub Release Publisher with dry-run and publish paths while preserving current release pipeline behavior.

## Architecture Status

Approved. The current release flow is:

- `release.py`
- `release/pipeline.py`
- `release/artifact.py`
- `release/manifest.py`
- `release/notes.py`
- `release/github.py`

## Remaining Work

- Implement R6R-4a Verification Profile Layer.
- Move verification scopes out of hardcoded release dev flow and task prompts.
- Keep backward compatibility for existing `--expect` and `--allow` CI usage.
- Exercise the GitHub publish path with a real release tag when the Architect approves an actual release.

## Next Implementation Step

Create data-driven verification profiles without changing firmware, runtime, SDK, demo runtime, build system, or release output helpers.
