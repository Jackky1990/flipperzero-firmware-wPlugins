from pathlib import Path

from .common import AEP


DEMO_SDK = AEP.parent / "astraeon_demo" / "lib" / "astraeon_sdk"
CANONICAL_SDK = AEP / "sdk"


def run_command():
    print("== AEP DEMO SDK DRIFT CHECK ==")

    failures = []

    for demo_file in sorted(DEMO_SDK.rglob("*")):
        if not demo_file.is_file():
            continue

        relative = demo_file.relative_to(DEMO_SDK)
        canonical_file = CANONICAL_SDK / relative

        if not canonical_file.exists():
            failures.append(f"missing canonical file: {relative}")
            continue

        if demo_file.read_bytes() != canonical_file.read_bytes():
            failures.append(f"demo SDK drift: {relative}")

    if failures:
        print("AEP DEMO SDK DRIFT CHECK FAILED")
        for failure in failures:
            print(f"- {failure}")
        return 1

    print("AEP DEMO SDK DRIFT CHECK PASSED")
    return 0
