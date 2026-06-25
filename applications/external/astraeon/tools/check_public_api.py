#!/usr/bin/env python3

from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
INCLUDE = ROOT / "sdk" / "include"

FAILED = False

for header in sorted(INCLUDE.glob("*.h")):
    text = header.read_text(encoding="utf-8")

    print(f"[CHECK] {header.name}")

    if "#pragma once" not in text:
        print("  ERROR: missing #pragma once")
        FAILED = True

    if "#ifdef __cplusplus" not in text:
        print("  ERROR: missing C++ guard")
        FAILED = True

    if "static " in text:
        print("  ERROR: static found in public header")
        FAILED = True

if FAILED:
    print("\nAPI CHECK FAILED")
    sys.exit(1)

print("\nAPI CHECK PASSED")
