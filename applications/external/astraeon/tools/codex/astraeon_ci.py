#!/usr/bin/env python3
import argparse
import subprocess
import sys

def run(cmd):
    print("[ASTRAEON CI] " + " ".join(cmd))
    result = subprocess.run(cmd)
    if result.returncode != 0:
        print("[ASTRAEON CI] FAIL")
        sys.exit(result.returncode)

def main():
    parser = argparse.ArgumentParser(description="ASTRAEON unified CI pipeline")
    parser.add_argument("--expect", action="append", default=[], help="Expected changed file exactly")
    parser.add_argument("--allow", action="append", default=[], help="Allowed changed file or directory prefix")
    parser.add_argument("--build-demo", action="store_true", help="Build ASTRAEON demo FAP")
    parser.add_argument("--clean", action="store_true", help="Require working tree clean")
    args = parser.parse_args()

    audit_cmd = ["python3", "applications/external/astraeon/tools/codex/safe_patch_audit.py"]

    if args.clean:
        audit_cmd.append("--clean")
    else:
        for item in args.expect:
            audit_cmd.extend(["--expect", item])
        for item in args.allow:
            audit_cmd.extend(["--allow", item])

    run(audit_cmd)

    if args.build_demo:
        run(["./fbt", "fap_astraeon_demo"])

    run(["python3", "applications/external/astraeon/tools/aep.py", "verify"])

    print("[ASTRAEON CI] PASS")

if __name__ == "__main__":
    main()
