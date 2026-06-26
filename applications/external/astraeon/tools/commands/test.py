from pathlib import Path
import subprocess
from .common import REPO, AEP

def run_command():
    print("== AEP RUNTIME TESTS ==")

    build_dir = REPO / "build" / "aep-host-tests"
    build_dir.mkdir(parents=True, exist_ok=True)

    output = build_dir / "aep_runtime_tests"

    sdk_src = sorted((AEP / "sdk" / "src").glob("*.c"))
    test_src = sorted((AEP / "tests" / "runtime").glob("*.c"))
    runner_src = sorted((AEP / "tests" / "runner").glob("*.c"))

    cmd = [
        "cc",
        "-std=c99",
        "-Wall",
        "-Wextra",
        "-Werror",
        "-I", str(AEP / "sdk" / "include"),
        "-I", str(AEP / "tests" / "include"),
        *[str(p) for p in sdk_src],
        *[str(p) for p in test_src],
        *[str(p) for p in runner_src],
        "-o", str(output),
    ]

    print("== HOST TEST BUILD ==")
    build = subprocess.run(cmd, cwd=REPO)
    if build.returncode != 0:
        return build.returncode

    print("== HOST TEST RUN ==")
    return subprocess.run([str(output)], cwd=REPO).returncode
