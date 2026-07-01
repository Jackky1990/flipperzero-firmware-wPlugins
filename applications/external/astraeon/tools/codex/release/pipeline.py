"""ASTRAEON release pipeline orchestration."""

import subprocess
import sys
from pathlib import Path

from release.artifact import RELEASE_ARTIFACTS, missing_artifacts
from release.github import publish_github_release
from release.manifest import build_manifest, write_manifest
from release.notes import write_release_notes


def release_output_dir(tag):
    return Path("build") / "astraeon-release" / tag


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

    manifest = build_manifest(args.tag)
    output_dir = release_output_dir(args.tag)
    manifest_path = write_manifest(manifest, output_dir / "manifest.json")
    notes_path = write_release_notes(manifest, output_dir / "release_notes.md")

    publish_github_release(
        args.tag,
        manifest,
        notes_path,
        manifest_path,
        dry_run=args.dry_run,
    )
