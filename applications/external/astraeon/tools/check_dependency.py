#!/usr/bin/env python3
from pathlib import Path
import json
import re
import sys

ROOT = Path(__file__).resolve().parents[1]
INCLUDE = ROOT / "sdk" / "include"
POLICY = ROOT / "tools" / "policy.json"

policy = json.loads(POLICY.read_text(encoding="utf-8"))

module_layer = {}
for layer, modules in policy["layers"].items():
    for module in modules:
        module_layer[module] = layer

forbidden = {(rule["from"], rule["to"]) for rule in policy["forbidden"]}

failed = False
include_re = re.compile(r'#include\s+"([^"]+)"')

for header in sorted(INCLUDE.glob("*.h")):
    module = header.stem
    from_layer = module_layer.get(module)

    print(f"[DEPS] {header.name}")

    if not from_layer:
        print(f"  WARN : module not listed in policy.json: {module}")
        continue

    text = header.read_text(encoding="utf-8")
    for inc in include_re.findall(text):
        target_module = Path(inc).stem
        to_layer = module_layer.get(target_module)

        if not to_layer:
            continue

        if (from_layer, to_layer) in forbidden:
            print(f"  ERROR: {from_layer} cannot include {to_layer}: {module} -> {target_module}")
            failed = True

if failed:
    print("\nDEPENDENCY CHECK FAILED")
    sys.exit(1)

print("\nDEPENDENCY CHECK PASSED")
