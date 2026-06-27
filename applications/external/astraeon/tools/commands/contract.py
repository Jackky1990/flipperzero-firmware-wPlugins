from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
SDK_INCLUDE = ROOT / "sdk" / "include"
SDK_SRC = ROOT / "sdk" / "src"
TESTS = ROOT / "tests" / "runtime"

FUNC_RE = re.compile(r"AstraResult\s+(astra_[a-zA-Z0-9_]+_context)\s*\(")

def run_command():
    print("== AEP CONTRACT SCAN ==")

    failures = []

    for header in sorted(SDK_INCLUDE.glob("astra_*.h")):
        text = header.read_text()
        funcs = FUNC_RE.findall(text)
        if not funcs:
            continue

        source = SDK_SRC / header.name.replace(".h", ".c")
        source_text = source.read_text() if source.exists() else ""

        for func in funcs:
            if f"{func}(" not in source_text:
                failures.append(f"missing implementation: {func} in {source}")

            test_hit = False
            for test in TESTS.glob("test_*.c"):
                if f"{func}(" in test.read_text():
                    test_hit = True
                    break

            if not test_hit:
                failures.append(f"missing test reference: {func}")

    if failures:
        print("AEP CONTRACT SCAN FAILED")
        for failure in failures:
            print(f"- {failure}")
        return 1

    print("AEP CONTRACT SCAN PASSED")
    return 0
