from pathlib import Path
import argparse
import json
import shutil
import subprocess
import sys

def die(msg):
    print(f"[SAFE PATCH] FAIL: {msg}")
    sys.exit(1)

def run(cmd):
    return subprocess.run(cmd, text=True)

def apply_patch(file_path, marker, insert, dry_run=False):
    target = Path(file_path)
    insert = insert.encode("utf-8").decode("unicode_escape")

    if not target.exists():
        die(f"file not found: {target}")

    text = target.read_text()
    marker_count = text.count(marker)

    if marker_count != 1:
        die(f"marker count must be 1, got {marker_count}")

    if insert.strip() in text:
        print(f"[SAFE PATCH] already applied: {target}")
        return None

    patched = text.replace(marker, marker + "\n" + insert, 1)

    if dry_run:
        print(f"[SAFE PATCH] dry run ok: {target}")
        return None

    backup = target.with_suffix(target.suffix + ".bak")
    shutil.copy2(target, backup)
    target.write_text(patched)

    print(f"[SAFE PATCH] patched: {target}")
    print(f"[SAFE PATCH] backup : {backup}")
    run(["git", "diff", "--", str(target)])
    return (target, backup)

parser = argparse.ArgumentParser()
parser.add_argument("spec", nargs="?")
parser.add_argument("--file")
parser.add_argument("--after")
parser.add_argument("--insert")
parser.add_argument("--dry-run", action="store_true")
parser.add_argument("--verify", action="store_true")
args = parser.parse_args()

patched_files = []

if args.spec:
    spec_path = Path(args.spec)
    if not spec_path.exists():
        die(f"spec not found: {spec_path}")

    spec = json.loads(spec_path.read_text())
    patches = spec.get("patches", [])
    verify = bool(spec.get("verify", False))
    dry_run = bool(spec.get("dry_run", False))

    if not patches:
        die("spec has no patches")

    for patch in patches:
        result = apply_patch(
            patch["file"],
            patch["after"],
            patch["insert"],
            dry_run=dry_run,
        )
        if result:
            patched_files.append(result)

    if verify and not dry_run:
        result = run(["python3", "applications/external/astraeon/tools/aep.py", "verify"])
        if result.returncode != 0:
            for target, backup in patched_files:
                shutil.copy2(backup, target)
            print("[SAFE PATCH] VERIFY FAILED")
            print("[SAFE PATCH] rollback applied")
            sys.exit(1)

        print("[SAFE PATCH] verify passed")

else:
    if not args.file or not args.after or not args.insert:
        die("usage: astra_patch.py <spec.json> OR --file <file> --after <marker> --insert <text>")

    result = apply_patch(args.file, args.after, args.insert, dry_run=args.dry_run)
    if result:
        patched_files.append(result)

    if args.verify and not args.dry_run:
        result = run(["python3", "applications/external/astraeon/tools/aep.py", "verify"])
        if result.returncode != 0:
            for target, backup in patched_files:
                shutil.copy2(backup, target)
            print("[SAFE PATCH] VERIFY FAILED")
            print("[SAFE PATCH] rollback applied")
            sys.exit(1)

        print("[SAFE PATCH] verify passed")
