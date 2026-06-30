#!/usr/bin/env python3
from pathlib import Path
import argparse
import subprocess
import sys

def run(cmd):
    print("[ASTRAEON RELEASE] " + " ".join(cmd))
    result = subprocess.run(cmd)
    if result.returncode != 0:
        print("[ASTRAEON RELEASE] FAIL")
        sys.exit(result.returncode)

def main():
    parser = argparse.ArgumentParser(description="ASTRAEON release pipeline")
    parser.add_argument("--tag", required=True)
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--dev", action="store_true", help="Allow testing while release.py is modified")
    args = parser.parse_args()

    fap = Path("build/f7-firmware-C/.extapps/astraeon_demo.fap")
    sdk = Path("dist/f7-C/flipper-z-f7-sdk-RM420FAP.zip")

    ci_cmd = [
        "python3",
        "applications/external/astraeon/tools/codex/astraeon_ci.py",
        "--build-demo",
    ]

    if args.dev:
        ci_cmd.extend([
            "--expect",
            "applications/external/astraeon/tools/codex/release.py",
        ])
    else:
        ci_cmd.append("--clean")

    run(ci_cmd)

    if not fap.exists():
        print(f"[ASTRAEON RELEASE] missing FAP: {fap}")
        sys.exit(1)

    if not sdk.exists():
        print(f"[ASTRAEON RELEASE] missing SDK bundle: {sdk}")
        sys.exit(1)

    print("[ASTRAEON RELEASE] artifacts ready")
    print(f"  tag: {args.tag}")
    print(f"  fap: {fap}")
    print(f"  sdk: {sdk}")

    if args.dry_run:
        print("[ASTRAEON RELEASE] dry run complete")
        return

    print("[ASTRAEON RELEASE] GitHub release creation not enabled yet")

if __name__ == "__main__":
    main()
