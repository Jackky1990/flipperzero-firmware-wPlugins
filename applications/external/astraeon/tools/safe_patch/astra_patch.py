from pathlib import Path
import sys
import shutil
import subprocess

def die(msg):
    print(f"[SAFE PATCH] FAIL: {msg}")
    sys.exit(1)

if len(sys.argv) != 4:
    die("usage: astra_patch.py <file> <needle> <insert_after>")

target = Path(sys.argv[1])
needle = sys.argv[2]
insert = sys.argv[3].encode("utf-8").decode("unicode_escape")

if not target.exists():
    die(f"file not found: {target}")

text = target.read_text()

if needle not in text:
    die("needle not found")

if insert.strip() in text:
    print("[SAFE PATCH] already applied")
    sys.exit(0)

backup = target.with_suffix(target.suffix + ".bak")
shutil.copy2(target, backup)

text = text.replace(needle, needle + "\n" + insert, 1)
target.write_text(text)

print(f"[SAFE PATCH] patched: {target}")
print(f"[SAFE PATCH] backup : {backup}")

subprocess.run(["git", "diff", "--", str(target)])
