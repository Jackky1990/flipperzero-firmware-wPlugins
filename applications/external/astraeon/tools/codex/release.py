#!/usr/bin/env python3
import argparse

from release.pipeline import run_release

def main():
    parser = argparse.ArgumentParser(description="ASTRAEON release pipeline")
    parser.add_argument("--tag", required=True)
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--dev", action="store_true", help="Allow testing while release.py is modified")
    args = parser.parse_args()

    run_release(args)

if __name__ == "__main__":
    main()
