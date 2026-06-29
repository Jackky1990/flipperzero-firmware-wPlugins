#!/usr/bin/env python3
from pathlib import Path
import argparse
import subprocess
import sys

def run(cmd):
    return subprocess.run(cmd, text=True, capture_output=True)

def changed_files():
    result = run(["git", "status", "--short"])
    if result.returncode != 0:
        print(result.stderr)
        sys.exit(1)

    files = []
    for line in result.stdout.splitlines():
        if not line.strip():
            continue
        path = line[3:].strip()
        if " -> " in path:
            path = path.split(" -> ", 1)[1]
        files.append(path)
    return files

def main():
    parser = argparse.ArgumentParser(description="ASTRAEON SAFE PATCH scope audit")
    parser.add_argument("--allow", action="append", default=[], help="Allowed changed file or directory prefix")
    parser.add_argument("--clean", action="store_true", help="Require working tree clean")
    args = parser.parse_args()

    files = changed_files()

    if args.clean:
        if files:
            print("[SAFE PATCH AUDIT] FAIL: working tree is not clean")
            for f in files:
                print(f"  {f}")
            return 1
        print("[SAFE PATCH AUDIT] PASS: working tree clean")
        return 0

    if not args.allow:
        print("[SAFE PATCH AUDIT] FAIL: no --allow scope provided")
        return 1

    unexpected = []
    for f in files:
        if not any(f == allowed or f.startswith(allowed.rstrip("/") + "/") for allowed in args.allow):
            unexpected.append(f)

    if unexpected:
        print("[SAFE PATCH AUDIT] FAIL: unexpected changed files")
        print("Allowed scopes:")
        for allowed in args.allow:
            print(f"  {allowed}")
        print("Unexpected:")
        for f in unexpected:
            print(f"  {f}")
        return 1

    print("[SAFE PATCH AUDIT] PASS")
    if files:
        print("Changed files:")
        for f in files:
            print(f"  {f}")
    else:
        print("Changed files: none")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
