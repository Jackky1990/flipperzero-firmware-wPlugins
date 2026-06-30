"""ASTRAEON release artifact helpers."""

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
