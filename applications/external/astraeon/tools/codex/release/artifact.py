"""ASTRAEON release artifact helpers."""

import hashlib
import shutil
from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class ReleaseArtifact:
    name: str
    path: Path


RELEASE_ARTIFACTS = (
    ReleaseArtifact("fap", Path("build/f7-firmware-C/.extapps/astraeon_demo.fap")),
    ReleaseArtifact("sdk", Path("dist/f7-C/flipper-z-f7-sdk-RM420FAP.zip")),
)


def missing_artifacts():
    return [artifact for artifact in RELEASE_ARTIFACTS if not artifact.path.exists()]


def sha256_file(path):
    artifact_path = Path(path)
    digest = hashlib.sha256()

    with artifact_path.open("rb") as artifact_file:
        for chunk in iter(lambda: artifact_file.read(1024 * 1024), b""):
            digest.update(chunk)

    return digest.hexdigest()


def verify_artifact(path):
    artifact_path = Path(path)

    if not artifact_path.is_file():
        raise FileNotFoundError(f"Artifact is missing: {artifact_path}")
    if artifact_path.stat().st_size == 0:
        raise ValueError(f"Artifact is empty: {artifact_path}")

    return artifact_path


def copy_artifact(src, dst):
    source_path = verify_artifact(src)
    destination_path = Path(dst)

    destination_path.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source_path, destination_path)

    return verify_artifact(destination_path)
