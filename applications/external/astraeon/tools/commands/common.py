from pathlib import Path
import subprocess

REPO = Path(__file__).resolve().parents[5]
AEP = REPO / "applications" / "external" / "astraeon"

def run(cmd):
    print(f"== {' '.join(cmd)} ==")
    return subprocess.run(cmd, cwd=REPO).returncode
