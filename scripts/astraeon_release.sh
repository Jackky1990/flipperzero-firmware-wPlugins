#!/usr/bin/env bash
set -euo pipefail

SCRIPT_NAME="$(basename "$0")"
DRY_RUN=0
ALLOW_MAIN=0
AUTO=0
COMMIT_HASH=""
PUSH_RESULT="not-run"
SYNC_RESULT="not-run"
VERIFICATION_RESULT="not-run"

usage() {
    cat <<USAGE
Usage:
  ./${SCRIPT_NAME} [--auto] [--dry-run] [--allow-main] "commit message"

Runs ASTRAEON verification, commits current changes, pushes the current branch,
and confirms the local branch is synchronized with origin.
USAGE
}

fail() {
    local step="$1"
    local reason="$2"
    printf '[ASTRAEON RELEASE] FAIL: %s: %s\n' "$step" "$reason" >&2
    exit 1
}

info() {
    printf '[ASTRAEON RELEASE] %s\n' "$1"
}

run_step() {
    local step="$1"
    shift
    info "RUN: ${step}"
    "$@" || fail "$step" "command failed"
}

print_report() {
    info "REPORT branch=${BRANCH:-unknown}"
    info "REPORT verification=${VERIFICATION_RESULT}"
    info "REPORT commit=${COMMIT_HASH:-none}"
    info "REPORT push=${PUSH_RESULT}"
    info "REPORT sync=${SYNC_RESULT}"
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --auto)
            AUTO=1
            shift
            ;;
        --dry-run)
            DRY_RUN=1
            shift
            ;;
        --allow-main)
            ALLOW_MAIN=1
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        --*)
            fail "argument parsing" "unknown option: $1"
            ;;
        *)
            break
            ;;
    esac
done

[[ $# -eq 1 ]] || {
    usage >&2
    fail "argument parsing" "commit message is required"
}

COMMIT_MESSAGE="$1"
[[ -n "$COMMIT_MESSAGE" ]] || fail "argument parsing" "commit message must not be empty"

git rev-parse --is-inside-work-tree >/dev/null 2>&1 || fail "repository check" "current directory is not inside a git repository"
REPO_ROOT="$(git rev-parse --show-toplevel)"
cd "$REPO_ROOT"

BRANCH="$(git branch --show-current)"
[[ -n "$BRANCH" ]] || fail "branch check" "detached HEAD is not supported"

if [[ "$ALLOW_MAIN" -ne 1 && ( "$BRANCH" == "main" || "$BRANCH" == "master" ) ]]; then
    fail "branch check" "refusing to run on ${BRANCH}; pass --allow-main to override"
fi

info "Repository: ${REPO_ROOT}"
info "Branch: ${BRANCH}"

CONFLICTS="$(git diff --name-only --diff-filter=U)"
[[ -z "$CONFLICTS" ]] || fail "working tree check" "merge conflict paths present"

STATUS_OUTPUT="$(git status --porcelain)"
if [[ -z "$STATUS_OUTPUT" ]]; then
    VERIFICATION_RESULT="skipped"
    PUSH_RESULT="skipped"
    SYNC_RESULT="skipped"
    info "nothing to release"
    print_report
    exit 0
fi

changed_paths() {
    git status --porcelain | while IFS= read -r line; do
        path="${line:3}"
        case "$path" in
            *" -> "*) path="${path##* -> }" ;;
        esac
        printf '%s\n' "$path"
    done
}

is_forbidden_path() {
    local path="$1"

    case "$path" in
        bootloader/*|targets/*/bootloader/*)
            return 0
            ;;
        fbt|fbt.cmd|SConstruct|site_scons/*|scripts/fbt/*|scripts/fbt_tools/*|applications/*/application.fam|applications/external/*/application.fam)
            return 0
            ;;
        firmware/*|applications/main/*|applications/services/*|applications/system/*|targets/*|lib/stm32*|lib/cmsis*|lib/STM32Cube*)
            return 0
            ;;
    esac

    return 1
}

FORBIDDEN=()
while IFS= read -r path; do
    if is_forbidden_path "$path"; then
        FORBIDDEN+=("$path")
    fi
done < <(changed_paths)

info "Changed files:"
while IFS= read -r path; do
    info "  ${path}"
done < <(changed_paths)

if [[ "${#FORBIDDEN[@]}" -gt 0 ]]; then
    printf '[ASTRAEON RELEASE] Forbidden modified paths:\n' >&2
    printf '  %s\n' "${FORBIDDEN[@]}" >&2
    fail "path policy" "auto release is blocked for firmware core, bootloader, or build-system changes"
fi

git remote get-url origin >/dev/null 2>&1 || fail "remote check" "origin remote is not configured"
run_step "remote reachability" git ls-remote --heads origin "$BRANCH"

SAFE_PATCH_AUDIT="applications/external/astraeon/tools/codex/safe_patch_audit.py"
AEP_TOOL="applications/external/astraeon/tools/aep.py"
ASTRAEON_CI="applications/external/astraeon/tools/codex/astraeon_ci.py"

SAFE_ALLOW_ARGS=(
    --allow .astraeon
    --allow scripts
    --allow applications/external/astraeon
    --allow applications/external/astraeon_demo
)

if [[ "$DRY_RUN" -eq 1 ]]; then
    info "DRY RUN: policy checks passed"
    [[ -f "$SAFE_PATCH_AUDIT" ]] && info "DRY RUN: would run Safe Patch Audit"
    [[ -f "$AEP_TOOL" ]] && info "DRY RUN: would run Runtime Tests, Architecture Audit, and AEP Verify"
    [[ -f "$ASTRAEON_CI" ]] && info "DRY RUN: would run ASTRAEON CI"
    info "DRY RUN: would commit with message: ${COMMIT_MESSAGE}"
    if [[ "$AUTO" -eq 1 ]]; then
        info "DRY RUN: would push origin ${BRANCH}"
        PUSH_RESULT="would-run"
        SYNC_RESULT="would-run"
    else
        info "DRY RUN: would stop after local commit; pass --auto to push"
        PUSH_RESULT="skipped"
        SYNC_RESULT="skipped"
    fi
    VERIFICATION_RESULT="would-run"
    print_report
    exit 0
fi

if [[ -f "$SAFE_PATCH_AUDIT" ]]; then
    run_step "Safe Patch Audit" python3 "$SAFE_PATCH_AUDIT" "${SAFE_ALLOW_ARGS[@]}"
else
    info "SKIP: Safe Patch Audit not found"
fi

if [[ -f "$AEP_TOOL" ]]; then
    run_step "Runtime Tests" python3 "$AEP_TOOL" test
    run_step "Architecture Audit" python3 "$AEP_TOOL" architecture
    run_step "AEP Verify" python3 "$AEP_TOOL" verify
else
    info "SKIP: AEP tool not found"
fi

if [[ -f "$ASTRAEON_CI" ]]; then
    run_step "ASTRAEON CI" python3 "$ASTRAEON_CI" "${SAFE_ALLOW_ARGS[@]}" --build-demo
else
    info "SKIP: ASTRAEON CI not found"
fi
VERIFICATION_RESULT="passed"

run_step "stage changes" git add -A
git diff --cached --quiet && fail "commit" "no staged changes after git add"
run_step "commit" git commit -m "$COMMIT_MESSAGE"
COMMIT_HASH="$(git rev-parse --short=10 HEAD)"

if [[ "$AUTO" -ne 1 ]]; then
    PUSH_RESULT="skipped"
    SYNC_RESULT="skipped"
    info "PASS: committed ${COMMIT_HASH}; pass --auto to push after verification"
    print_report
    exit 0
fi

info "RUN: push"
if ! git push origin "$BRANCH"; then
    PUSH_RESULT="failed"
    SYNC_RESULT="not-run"
    printf '[ASTRAEON RELEASE] FAIL: push: command failed\n' >&2
    printf '[ASTRAEON RELEASE] Recovery command: git push origin %s\n' "$BRANCH" >&2
    print_report
    exit 1
fi
PUSH_RESULT="passed"

run_step "fetch pushed branch" git fetch origin "$BRANCH"

LOCAL_HEAD="$(git rev-parse HEAD)"
REMOTE_HEAD="$(git rev-parse "origin/${BRANCH}")"
[[ "$LOCAL_HEAD" == "$REMOTE_HEAD" ]] || fail "sync check" "local ${LOCAL_HEAD} differs from origin/${BRANCH} ${REMOTE_HEAD}"

SYNC_RESULT="passed"
info "PASS: local branch is synchronized with origin/${BRANCH}"
print_report
