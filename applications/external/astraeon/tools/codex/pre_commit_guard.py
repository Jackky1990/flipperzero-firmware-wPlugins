#!/usr/bin/env python3
import argparse
import subprocess
import sys

def run(cmd):
    print("[GUARD] " + " ".join(cmd))
    result = subprocess.run(cmd)
    if result.returncode != 0:
        sys.exit(result.returncode)

parser = argparse.ArgumentParser(description="ASTRAEON pre-commit guard")
parser.add_argument("--expect", action="append", default=[])
parser.add_argument("--allow", action="append", default=[])
args = parser.parse_args()

audit_cmd = ["python3", "applications/external/astraeon/tools/codex/safe_patch_audit.py"]

for item in args.expect:
    audit_cmd.extend(["--expect", item])

for item in args.allow:
    audit_cmd.extend(["--allow", item])

run(audit_cmd)
run(["python3", "applications/external/astraeon/tools/aep.py", "verify"])
print("[GUARD] PASS")
