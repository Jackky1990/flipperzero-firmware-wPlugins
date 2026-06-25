from .common import run

def run_command():
    rc = 0
    rc |= run(["python3", "--version"])
    rc |= run(["git", "--version"])
    rc |= run(["git", "branch", "--show-current"])
    rc |= run(["test", "-x", "./fbt"])
    print("\nAEP DOCTOR PASSED" if rc == 0 else "\nAEP DOCTOR FAILED")
    return rc
