from . import doctor, check_api, check_deps, check_demo_sdk, build, status, test, contract, architecture

def run_command():
    if doctor.run_command() != 0:
        return 1
    if check_api.run_command() != 0:
        return 1
    if check_deps.run_command() != 0:
        return 1
    if check_demo_sdk.run_command() != 0:
        return 1
    if build.run_command() != 0:
        return 1
    if test.run_command() != 0:
        return 1
    if contract.run_command() != 0:
        return 1
    if architecture.run_command() != 0:
        return 1
    status.run_command()
    print("\nAEP VERIFY PASSED")
    return 0
