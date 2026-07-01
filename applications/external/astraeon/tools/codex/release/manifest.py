"""ASTRAEON release manifest helpers."""

import json
from pathlib import Path

from release.artifact import RELEASE_ARTIFACTS, sha256_file, verify_artifact


def artifact_manifest_entry(artifact):
    artifact_path = verify_artifact(artifact.path)

    return {
        "name": artifact.name,
        "path": str(artifact_path),
        "size": artifact_path.stat().st_size,
        "sha256": sha256_file(artifact_path),
    }


def build_manifest(tag, artifacts=RELEASE_ARTIFACTS):
    return {
        "tag": tag,
        "artifacts": [artifact_manifest_entry(artifact) for artifact in artifacts],
    }


def write_manifest(manifest, path):
    manifest_path = Path(path)
    manifest_path.parent.mkdir(parents=True, exist_ok=True)
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    return manifest_path
