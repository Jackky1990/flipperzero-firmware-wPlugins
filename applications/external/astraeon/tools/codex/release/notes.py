"""ASTRAEON release notes helpers."""

from pathlib import Path


def render_release_notes(manifest):
    lines = [
        f"# ASTRAEON Release {manifest['tag']}",
        "",
        "## Artifacts",
        "",
    ]

    for artifact in manifest["artifacts"]:
        lines.extend([
            f"- {artifact['name']}",
            f"  - Path: `{artifact['path']}`",
            f"  - Size: {artifact['size']} bytes",
            f"  - SHA256: `{artifact['sha256']}`",
        ])

    return "\n".join(lines) + "\n"


def write_release_notes(manifest, path):
    notes_path = Path(path)
    notes_path.parent.mkdir(parents=True, exist_ok=True)
    notes_path.write_text(render_release_notes(manifest), encoding="utf-8")
    return notes_path
