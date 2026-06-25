from .common import run

def run_command():
    run(["git", "status", "--short"])
    run(["git", "branch", "--show-current"])
    return 0
