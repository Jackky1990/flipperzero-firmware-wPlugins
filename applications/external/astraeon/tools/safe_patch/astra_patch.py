from pathlib import Path
import argparse
import shutil
import subprocess
import sys

def die(msg):
    print(f"[SAFE PATCH] FAIL: {msg}")
    sys.exit(1)

def run(cmd):
    return subprocess.run(cmd, text=True)

parser = argparse.ArgumentParser()
parser.add_argument("--file", required=True)
parser.add_argument("--after", required=True)
parser.add_argument("--insert", required=True)
parser.add_argument("--dry-run", action="store_true")
parser.add_argument("--verify", action="store_true")
args = parser.parse_args()

target = Path(args.file)
needle = args.after
insert = args.insert.encode("utf-8").decode("unicode_escape")

if not target.exists():
    die(f"file not found: {target}")

text = target.read_text()

if needle not in text:
    die("marker not found")

if insert.strip() in text:
    print("[SAFE PATCH] already applied")
    sys.exit(0)

patched = text.replace(needle, needle + "\n" + insert, 1)

if args.dry_run:
    print("[SAFE PATCH] dry run ok")
    print(f"[SAFE PATCH] target: {target}")
    sys.exit(0)

backup = target.with_suffix(target.suffix + ".bak")
shutil.copy2(target, backup)

target.write_text(patched)

print(f"[SAFE PATCH] patched: {target}")
print(f"[SAFE PATCH] backup : {backup}")

run(["git", "diff", "--", str(target)])

if args.verify:
    result = run(["python3", "applications/external/astraeon/tools/aep.py", "verify"])
    if result.returncode != 0:
        shutil.copy2(backup, target)
        print("[SAFE PATCH] VERIFY FAILED")
        print("[SAFE PATCH] rollback applied")
        sys.exit(1)

    print("[SAFE PATCH] verify passed")
