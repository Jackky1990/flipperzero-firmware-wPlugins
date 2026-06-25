from . import doctor, check_api, check_deps, build, status

def run_command():
    if doctor.run_command() != 0:
        return 1
    if check_api.run_command() != 0:
        return 1
    if check_deps.run_command() != 0:
        return 1
    if build.run_command() != 0:
        return 1
    status.run_command()
    print("\nAEP VERIFY PASSED")
    return 0
