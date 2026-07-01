"""ASTRAEON release pipeline orchestration."""

import subprocess
import sys

from release.artifact import RELEASE_ARTIFACTS, missing_artifacts


def run(cmd):
    print("[ASTRAEON RELEASE] " + " ".join(cmd))
    result = subprocess.run(cmd)
    if result.returncode != 0:
        print("[ASTRAEON RELEASE] FAIL")
        sys.exit(result.returncode)


def run_release(args):
    ci_cmd = [
        "python3",
        "applications/external/astraeon/tools/codex/astraeon_ci.py",
        "--build-demo",
    ]

    if args.dev:
        ci_cmd.extend([
            "--expect",
            "applications/external/astraeon/tools/codex/release.py",
            "--allow",
            "applications/external/astraeon/tools/codex/release",
        ])
    else:
        ci_cmd.append("--clean")

    run(ci_cmd)

    missing = missing_artifacts()
    if missing:
        for artifact in missing:
            print(f"[ASTRAEON RELEASE] missing {artifact.name}: {artifact.path}")
        sys.exit(1)

    print("[ASTRAEON RELEASE] artifacts ready")
    print(f"  tag: {args.tag}")
    for artifact in RELEASE_ARTIFACTS:
        print(f"  {artifact.name}: {artifact.path}")

    if args.dry_run:
        print("[ASTRAEON RELEASE] dry run complete")
        return

    print("[ASTRAEON RELEASE] GitHub release creation not enabled yet")
