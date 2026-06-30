from pathlib import Path

from .common import AEP


DEMO_SDK = AEP.parent / "astraeon_demo" / "lib" / "astraeon_sdk"
CANONICAL_SDK = AEP / "sdk"
DEMO_APPLICATION = AEP.parent / "astraeon_demo" / "application.fam"

REQUIRED_DEMO_APPLICATION_TOKENS = (
    "ASTRAEON_DEMO_USB_CDC = 0",
    "ASTRA_RUNTIME_ENABLE_USB_CDC",
    "cdefines=ASTRAEON_DEMO_CDEFINES",
    "src/astra_runtime_usb_transport.c",
    "src/astra_node01.c",
    "src/astra_node01_usb_bridge.c",
)

REQUIRED_DEMO_SDK_FILES = (
    "include/astra_event.h",
    "include/astra_event_builder.h",
    "include/astra_event_persistence.h",
    "include/astra_logger.h",
    "include/astra_node01.h",
    "include/astra_node01_usb_bridge.h",
    "include/astra_policy.h",
    "include/astra_runtime_config.h",
    "include/astra_runtime_usb_transport.h",
    "include/astra_storage.h",
    "src/astra_event.c",
    "src/astra_event_builder.c",
    "src/astra_event_persistence.c",
    "src/astra_logger.c",
    "src/astra_node01.c",
    "src/astra_node01_usb_bridge.c",
    "src/astra_policy.c",
    "src/astra_runtime_transport.c",
    "src/astra_runtime_usb_transport.c",
    "src/astra_storage.c",
)


def run_command():
    print("== AEP DEMO SDK DRIFT CHECK ==")

    failures = []

    for relative_name in REQUIRED_DEMO_SDK_FILES:
        relative = Path(relative_name)
        demo_file = DEMO_SDK / relative
        canonical_file = CANONICAL_SDK / relative

        if not demo_file.exists():
            failures.append(f"missing demo SDK file: {relative}")

        if not canonical_file.exists():
            failures.append(f"missing canonical SDK file: {relative}")

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

    application_manifest = DEMO_APPLICATION.read_text(encoding="utf-8")
    for token in REQUIRED_DEMO_APPLICATION_TOKENS:
        if token not in application_manifest:
            failures.append(f"demo application manifest missing: {token}")

    if failures:
        print("AEP DEMO SDK DRIFT CHECK FAILED")
        for failure in failures:
            print(f"- {failure}")
        return 1

    print("AEP DEMO SDK DRIFT CHECK PASSED")
    return 0
