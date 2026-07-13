#!/usr/bin/env python3
import argparse
import subprocess
import sys

from safe_patch_audit import scope_arguments

def run(cmd):
    print("[GUARD] " + " ".join(cmd))
    result = subprocess.run(cmd)
    if result.returncode != 0:
        sys.exit(result.returncode)

def main():
    parser = argparse.ArgumentParser(description="ASTRAEON pre-commit guard")
    parser.add_argument("--expect", action="append", default=[])
    parser.add_argument("--allow", action="append", default=[])
    args = parser.parse_args()

    audit_cmd = ["python3", "applications/external/astraeon/tools/codex/safe_patch_audit.py"]
    audit_cmd.extend(scope_arguments(args.expect, args.allow))

    run(audit_cmd)
    run(["python3", "applications/external/astraeon/tools/aep.py", "verify"])
    print("[GUARD] PASS")


if __name__ == "__main__":
    main()
