#!/usr/bin/env python3
"""Bump the tabulate version number everywhere it is recorded.

Releases are MAJOR.MINOR only (no patch component), e.g. 1.0, 2.1, 2.2.
Updates (and regenerates the single-include header) so a release can never
miss a spot:
  - include/tabulate/tabulate.hpp   (TABULATE_VERSION_{MAJOR,MINOR}, PATCH stays 0)
  - single_include/tabulate/tabulate.hpp (regenerated via amalgamate)
  - CMakeLists.txt                 (project(tabulate VERSION x.y ...))
  - README.md                      (version-x.y-blue badge)

Usage:
  ./bump-version.py 1.6
  ./bump-version.py 1.6 --dry-run
"""
import argparse
import re
import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent

VERSION_RE = re.compile(r"^(\d+)\.(\d+)$")


class VersionBumpError(RuntimeError):
    pass


def read_current_version(tabulate_hpp: Path) -> str:
    text = tabulate_hpp.read_text()
    major = re.search(r"#define TABULATE_VERSION_MAJOR (\d+)", text)
    minor = re.search(r"#define TABULATE_VERSION_MINOR (\d+)", text)
    if not (major and minor):
        raise VersionBumpError(f"Could not find TABULATE_VERSION_* defines in {tabulate_hpp}")
    return f"{major.group(1)}.{minor.group(1)}"


def substitute_once(path: Path, pattern: str, replacement: str, dry_run: bool) -> None:
    text = path.read_text()
    new_text, count = re.subn(pattern, replacement, text, count=1)
    if count != 1:
        raise VersionBumpError(f"Expected exactly one match for {pattern!r} in {path}, found {count}")
    if not dry_run:
        path.write_text(new_text)
    print(f"  updated {path.relative_to(REPO_ROOT)}")


def update_version_defines(path: Path, major: str, minor: str, dry_run: bool) -> None:
    text = path.read_text()
    new_text, n1 = re.subn(r"#define TABULATE_VERSION_MAJOR \d+", f"#define TABULATE_VERSION_MAJOR {major}", text, count=1)
    new_text, n2 = re.subn(r"#define TABULATE_VERSION_MINOR \d+", f"#define TABULATE_VERSION_MINOR {minor}", new_text, count=1)
    # Releases have no patch component; keep it pinned at 0.
    new_text, n3 = re.subn(r"#define TABULATE_VERSION_PATCH \d+", "#define TABULATE_VERSION_PATCH 0", new_text, count=1)
    if (n1, n2, n3) != (1, 1, 1):
        raise VersionBumpError(f"Could not update all TABULATE_VERSION_* defines in {path}")
    if not dry_run:
        path.write_text(new_text)
    print(f"  updated {path.relative_to(REPO_ROOT)}")


def regenerate_single_include(dry_run: bool) -> None:
    if dry_run:
        print("  (dry-run) would regenerate single_include/tabulate/tabulate.hpp via amalgamate.py")
        return
    result = subprocess.run(
        ["python3", "utils/amalgamate/amalgamate.py", "-c", "single_include.json", "-s", "."],
        cwd=REPO_ROOT,
        capture_output=True,
        text=True,
    )
    if result.returncode != 0:
        raise VersionBumpError(
            "Failed to regenerate single_include/tabulate/tabulate.hpp via amalgamate.py:\n"
            f"{result.stdout}\n{result.stderr}"
        )
    print("  regenerated single_include/tabulate/tabulate.hpp")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("version", help="new version, e.g. 1.6")
    parser.add_argument("--dry-run", action="store_true", help="show what would change without writing files")
    args = parser.parse_args()

    match = VERSION_RE.match(args.version)
    if not match:
        print(f"error: version must look like MAJOR.MINOR (got {args.version!r})", file=sys.stderr)
        return 1
    major, minor = match.groups()

    tabulate_hpp = REPO_ROOT / "include" / "tabulate" / "tabulate.hpp"
    single_include_hpp = REPO_ROOT / "single_include" / "tabulate" / "tabulate.hpp"
    cmakelists = REPO_ROOT / "CMakeLists.txt"
    readme = REPO_ROOT / "README.md"

    try:
        old_version = read_current_version(tabulate_hpp)
        print(f"Bumping version: {old_version} -> {args.version}" + (" (dry run)" if args.dry_run else ""))

        update_version_defines(tabulate_hpp, major, minor, args.dry_run)

        substitute_once(
            cmakelists,
            r"project\(tabulate VERSION \d+\.\d+ LANGUAGES CXX\)",
            f"project(tabulate VERSION {args.version} LANGUAGES CXX)",
            args.dry_run,
        )

        substitute_once(
            readme,
            r"version-\d+\.\d+-blue",
            f"version-{major}.{minor}-blue",
            args.dry_run,
        )

        # Regenerate the amalgamated header so it never drifts from include/tabulate.
        regenerate_single_include(args.dry_run)
        if not args.dry_run:
            regenerated_version = read_current_version(single_include_hpp)
            if regenerated_version != args.version:
                raise VersionBumpError(
                    f"single_include/tabulate/tabulate.hpp has version {regenerated_version} "
                    f"after regeneration, expected {args.version}"
                )
    except VersionBumpError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1

    print("Done." if not args.dry_run else "Dry run complete, no files changed.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
