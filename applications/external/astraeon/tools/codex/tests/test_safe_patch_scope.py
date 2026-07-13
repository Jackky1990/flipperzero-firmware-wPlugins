#!/usr/bin/env python3
import contextlib
import importlib.util
import io
from pathlib import Path
import runpy
import subprocess
import sys
import unittest
from unittest import mock


CODEX_TOOLS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(CODEX_TOOLS))


def load_module(name, filename):
    spec = importlib.util.spec_from_file_location(name, CODEX_TOOLS / filename)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


class ScopeForwardingTests(unittest.TestCase):
    def completed(self, cmd, **kwargs):
        self.commands.append(cmd)
        return subprocess.CompletedProcess(cmd, 0)

    def setUp(self):
        self.commands = []

    def test_ci_defaults_to_clean_scope(self):
        module = load_module("astraeon_ci_test", "astraeon_ci.py")
        with mock.patch.object(sys, "argv", ["astraeon_ci.py", "--build-demo"]):
            with mock.patch.object(module.subprocess, "run", side_effect=self.completed):
                module.main()

        self.assertEqual(self.commands[0][-1], "--clean")

    def test_ci_forwards_explicit_scope(self):
        module = load_module("astraeon_ci_scoped_test", "astraeon_ci.py")
        argv = ["astraeon_ci.py", "--allow", "approved", "--expect", "approved/file.py"]
        with mock.patch.object(sys, "argv", argv):
            with mock.patch.object(module.subprocess, "run", side_effect=self.completed):
                module.main()

        self.assertEqual(
            self.commands[0][2:],
            ["--expect", "approved/file.py", "--allow", "approved"],
        )

    def test_guard_defaults_to_clean_scope(self):
        with mock.patch.object(sys, "argv", ["pre_commit_guard.py"]):
            with mock.patch("subprocess.run", side_effect=self.completed):
                runpy.run_path(str(CODEX_TOOLS / "pre_commit_guard.py"), run_name="__main__")

        self.assertEqual(self.commands[0][-1], "--clean")


class SafePatchStrictnessTests(unittest.TestCase):
    def setUp(self):
        self.module = load_module("safe_patch_audit_test", "safe_patch_audit.py")

    def audit(self, changed, *args):
        output = io.StringIO()
        with mock.patch.object(self.module, "changed_files", return_value=changed):
            with mock.patch.object(sys, "argv", ["safe_patch_audit.py", *args]):
                with contextlib.redirect_stdout(output):
                    result = self.module.main()
        return result, output.getvalue()

    def test_changed_files_expands_untracked_directories(self):
        def git_status(cmd):
            path = "new/tests/test_scope.py" if "--untracked-files=all" in cmd else "new/"
            return subprocess.CompletedProcess(cmd, 0, stdout=f"?? {path}\n", stderr="")

        with mock.patch.object(self.module, "run", side_effect=git_status):
            self.assertEqual(self.module.changed_files(), ["new/tests/test_scope.py"])

    def test_passes_when_every_file_is_in_scope(self):
        result, _ = self.audit(["approved/file.py"], "--allow", "approved")
        self.assertEqual(result, 0)

    def test_fails_when_any_file_is_outside_scope(self):
        result, output = self.audit(
            ["approved/file.py", "outside/file.py"],
            "--allow",
            "approved",
        )
        self.assertEqual(result, 1)
        self.assertIn("outside/file.py", output)


if __name__ == "__main__":
    unittest.main()
