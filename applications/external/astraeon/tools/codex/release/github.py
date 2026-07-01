"""ASTRAEON GitHub release publisher."""

import subprocess
import sys


def release_assets(manifest, manifest_path, notes_path):
    assets = [artifact["path"] for artifact in manifest["artifacts"]]
    assets.extend([str(manifest_path), str(notes_path)])
    return assets


def publish_github_release(tag, manifest, notes_path, manifest_path, dry_run=False):
    assets = release_assets(manifest, manifest_path, notes_path)

    if dry_run:
        print("[ASTRAEON RELEASE] GitHub release dry run")
        print(f"  tag: {tag}")
        print(f"  notes: {notes_path}")
        for asset in assets:
            print(f"  asset: {asset}")
        return

    cmd = [
        "gh",
        "release",
        "create",
        tag,
        "--title",
        tag,
        "--notes-file",
        str(notes_path),
    ]
    cmd.extend(assets)

    print("[ASTRAEON RELEASE] " + " ".join(cmd))
    result = subprocess.run(cmd)
    if result.returncode != 0:
        print("[ASTRAEON RELEASE] GitHub release publish failed")
        sys.exit(result.returncode)

    print("[ASTRAEON RELEASE] GitHub release published")
