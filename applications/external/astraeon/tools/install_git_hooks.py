from pathlib import Path
import os
import shutil
import sys

def find_git_root(start: Path) -> Path:
    current = start.resolve()

    while True:
        if (current / ".git").exists():
            return current

        if current.parent == current:
            raise RuntimeError(
                "Git repository root (.git) not found."
            )

        current = current.parent


ROOT = find_git_root(Path(__file__).parent)

HOOK_DIR = ROOT / ".git" / "hooks"
HOOK_DIR.mkdir(parents=True, exist_ok=True)

HOOK = HOOK_DIR / "pre-commit"

if HOOK.exists():
    backup = HOOK.with_suffix(".bak")
    shutil.copy2(HOOK, backup)
    print(f"Existing hook backed up -> {backup}")

script = """#!/bin/sh

echo "[AEP] Running verify..."

python3 applications/external/astraeon/tools/aep.py verify

STATUS=$?

if [ $STATUS -ne 0 ]; then
    echo ""
    echo "[AEP] VERIFY FAILED"
    echo "[AEP] Commit rejected."
    exit 1
fi

echo "[AEP] VERIFY PASSED"

exit 0
"""

HOOK.write_text(script)

os.chmod(HOOK, 0o755)

print()
print("Git root :", ROOT)
print("Hook     :", HOOK)
print()
print("Git Guard installed successfully.")
