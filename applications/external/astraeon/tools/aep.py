#!/usr/bin/env python3
import sys
from commands import doctor, check_api, check_deps, check_demo_sdk, build, status, verify, test, contract, architecture

COMMANDS = {
    "doctor": doctor.run_command,
    "check-api": check_api.run_command,
    "check-deps": check_deps.run_command,
    "check-demo-sdk": check_demo_sdk.run_command,
    "build": build.run_command,
    "status": status.run_command,
    "verify": verify.run_command,
    "test": test.run_command,
    "contract": contract.run_command,
    "architecture": architecture.run_command,
}

def main():
    if len(sys.argv) < 2:
        print("usage: aep.py [doctor|check-api|check-deps|check-demo-sdk|build|status|test|contract|architecture|verify]")
        return 2

    cmd = sys.argv[1]
    if cmd not in COMMANDS:
        print(f"unknown command: {cmd}")
        return 2

    return COMMANDS[cmd]()

if __name__ == "__main__":
    raise SystemExit(main())
