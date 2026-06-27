from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
SDK_INCLUDE = ROOT / "sdk" / "include"
SDK_SRC = ROOT / "sdk" / "src"
TESTS = ROOT / "tests" / "runtime"

DECL_RE = re.compile(
    r"(?:AstraResult|AstraEventHandler|AstraRuntimeContext\*|void\*)\s+"
    r"(astra_[A-Za-z0-9_]*context[A-Za-z0-9_]*)\s*\("
)

WRAPPER_MODULES = [
    "astra_registry",
    "astra_scheduler",
    "astra_event_bus",
    "astra_dispatcher",
    "astra_runtime_loop",
]

def count_definitions(func):
    pattern = re.compile(
        r"(?:AstraResult|AstraEventHandler|AstraRuntimeContext\*|void\*)\s+"
        + re.escape(func) +
        r"\s*\("
    )
    count = 0
    for source in SDK_SRC.glob("astra_*.c"):
        count += len(pattern.findall(source.read_text()))
    return count

def run_command():
    print("== AEP CONTRACT SCAN v2 ==")

    failures = []

    for header in sorted(SDK_INCLUDE.glob("astra_*.h")):
        funcs = sorted(set(DECL_RE.findall(header.read_text())))
        if not funcs:
            continue

        source = SDK_SRC / header.name.replace(".h", ".c")
        source_text = source.read_text() if source.exists() else ""

        for func in funcs:
            if f"{func}(" not in source_text:
                failures.append(f"missing implementation: {func}")

            definitions = count_definitions(func)
            if definitions > 1:
                failures.append(f"duplicate implementation: {func} ({definitions})")

            if not any(f"{func}(" in t.read_text() for t in TESTS.glob("test_*.c")):
                failures.append(f"missing test reference: {func}")

    for module in WRAPPER_MODULES:
        source = SDK_SRC / f"{module}.c"
        if not source.exists():
            failures.append(f"missing wrapper source: {module}.c")
            continue

        text = source.read_text()
        if "astra_runtime_default_context()" not in text:
            failures.append(f"wrapper does not use default runtime context: {module}")

    if failures:
        print("AEP CONTRACT SCAN FAILED")
        for failure in failures:
            print(f"- {failure}")
        return 1

    print("AEP CONTRACT SCAN PASSED")
    print(f"Wrapper coverage: {len(WRAPPER_MODULES)} / {len(WRAPPER_MODULES)}")
    return 0
