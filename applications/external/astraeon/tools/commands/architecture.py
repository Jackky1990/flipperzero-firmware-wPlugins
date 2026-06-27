from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
SDK_SRC = ROOT / "sdk" / "src"

MODULES = [
    "astra_registry",
    "astra_scheduler",
    "astra_event_bus",
    "astra_dispatcher",
    "astra_runtime_loop",
]

GLOBAL_PATTERNS = [
    r"^static\s+.*default_",
    r"^static\s+.*initialized",
]

def run_command():
    print("== AEP ARCHITECTURE AUDIT ==")

    wrapper_ok = 0
    globals_found = 0

    for module in MODULES:
        path = SDK_SRC / f"{module}.c"
        if not path.exists():
            continue

        text = path.read_text()

        if "astra_runtime_default_context()" in text:
            wrapper_ok += 1

        for pattern in GLOBAL_PATTERNS:
            globals_found += len(
                re.findall(pattern, text, re.MULTILINE)
            )

    score = int((wrapper_ok / len(MODULES)) * 100)

    print(f"Architecture Score : {score}%")
    print(f"Wrapper Coverage   : {wrapper_ok}/{len(MODULES)}")
    print(f"Global State Count : {globals_found}")

    if wrapper_ok != len(MODULES):
        print("ARCHITECTURE AUDIT FAILED")
        return 1

    print("ARCHITECTURE AUDIT PASSED")
    return 0
