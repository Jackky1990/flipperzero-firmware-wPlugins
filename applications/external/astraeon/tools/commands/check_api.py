from .common import run, AEP

def run_command():
    return run(["python3", str(AEP / "tools" / "check_public_api.py")])
