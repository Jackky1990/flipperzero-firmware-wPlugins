#!/usr/bin/env python3
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
INCLUDE = ROOT / "sdk" / "include"
SRC = ROOT / "sdk" / "src"
TESTS = ROOT / "tests" / "runtime"

def module_suffix(name: str) -> str:
    return name.removeprefix("astra_")

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
    test = TESTS / f"test_{module_suffix(name)}.c"

    if header.exists() or source.exists() or test.exists():
        print("error: module already exists")
        return 1

    header.write_text(f"""#pragma once

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {{
#endif

AstraResult {name}_init(void);

#ifdef __cplusplus
}}
#endif
""", encoding="utf-8")

    source.write_text(f"""#include "{name}.h"

AstraResult {name}_init(void) {{
    return astra_result_ok();
}}
""", encoding="utf-8")

    test.write_text(f"""#include "astra_tests.h"
#include "{name}.h"

bool astra_test_{module_suffix(name)}(void) {{
    return {name}_init().status == AstraStatusOk;
}}
""", encoding="utf-8")

    print(f"created: {header}")
    print(f"created: {source}")
    print(f"created: {test}")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
