#!/usr/bin/env python3
import sys
from commands import doctor, check_api, check_deps, build, status, verify, test

COMMANDS = {
    "doctor": doctor.run_command,
    "check-api": check_api.run_command,
    "check-deps": check_deps.run_command,
    "build": build.run_command,
    "status": status.run_command,
    "verify": verify.run_command,
    "test": test.run_command,
}

def main():
    if len(sys.argv) < 2:
        print("usage: aep.py [doctor|check-api|check-deps|build|status|test|verify]")
        return 2

    cmd = sys.argv[1]
    if cmd not in COMMANDS:
        print(f"unknown command: {cmd}")
        return 2

    return COMMANDS[cmd]()

if __name__ == "__main__":
    raise SystemExit(main())
