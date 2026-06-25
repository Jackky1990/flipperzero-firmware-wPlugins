#!/usr/bin/env python3
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
INCLUDE = ROOT / "sdk" / "include"
SRC = ROOT / "sdk" / "src"

def guard_name(name: str) -> str:
    return name.upper().replace("-", "_")

def main() -> int:
    if len(sys.argv) != 2:
        print("usage: create_module.py astra_module_name")
        return 2

    name = sys.argv[1].strip()
    if not name.startswith("astra_"):
        print("error: module name must start with astra_")
        return 2

    header = INCLUDE / f"{name}.h"
    source = SRC / f"{name}.c"

    if header.exists() or source.exists():
        print("error: module already exists")
        return 1

    header.write_text(f"""#pragma once

#ifdef __cplusplus
extern "C" {{
#endif

/* {name}: public API */

#ifdef __cplusplus
}}
#endif
""", encoding="utf-8")

    source.write_text(f"""#include "{name}.h"

/* {name}: implementation */
""", encoding="utf-8")

    print(f"created: {header}")
    print(f"created: {source}")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
