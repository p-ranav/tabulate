#!/usr/bin/env bash
# Measures source-line coverage for the whole test suite as ONE combined
# binary instead of merging .profraw files from many separately-compiled
# test binaries. Merging across binaries that all include the same
# header-only inline code produces unreliable/wildly wrong per-line counts
# ("N functions have mismatched data" from llvm-profdata is a strong sign of
# this). Concatenating every test's main() into one translation unit and
# profiling that single binary avoids the problem entirely.
#
# Usage:
#   test/coverage.bash                              # full per-file report
#   test/coverage.bash include/tabulate/row.hpp      # annotated source with
#                                                     # per-line hit counts
#
# Requires: g++ (or clang++) with -fprofile-instr-generate support, and
# Xcode command line tools (xcrun llvm-profdata / llvm-cov) on macOS, or
# llvm-profdata / llvm-cov on PATH elsewhere.
set -e

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
work_dir="$(mktemp -d /tmp/tabulate_coverage.XXXXXX)"
trap 'rm -rf "$work_dir"' EXIT

if command -v xcrun >/dev/null 2>&1; then
  llvm_profdata=(xcrun llvm-profdata)
  llvm_cov=(xcrun llvm-cov)
else
  llvm_profdata=(llvm-profdata)
  llvm_cov=(llvm-cov)
fi

cd "$repo_root/test"

python3 - "$work_dir" << 'PYEOF'
import re, glob, os, sys

work_dir = sys.argv[1]
preamble_includes = set()
bodies = []
names = []
for path in sorted(glob.glob("*.cpp")):
    name = os.path.splitext(path)[0]
    fn_name = f"run_{name}"
    names.append(name)
    text = open(path).read()
    text = re.sub(r'#ifdef TABULATE_SINGLE_HEADER.*?#endif\n', '', text, count=1, flags=re.S)
    def collect_include(m):
        preamble_includes.add(m.group(0))
        return ''
    text = re.sub(r'#include\s*[<"][^>"]+[>"]\n', collect_include, text)
    text = re.sub(r'\bint\s+main\s*\(\s*\)', f'int {fn_name}()', text, count=1)
    bodies.append(f'namespace ns_{name} {{\n{text}\n}}\n\n')

out = ['#ifdef TABULATE_SINGLE_HEADER\n#include <tabulate/tabulate.hpp>\n#else\n'
       '#include <tabulate/table.hpp>\n#include <tabulate/markdown_exporter.hpp>\n'
       '#include <tabulate/latex_exporter.hpp>\n#include <tabulate/asciidoc_exporter.hpp>\n'
       '#endif\n']
out.extend(sorted(preamble_includes))
out.append('\n')
out.extend(bodies)
out.append('int main() {\n  int failures = 0;\n')
for name in names:
    out.append(f'  failures += ns_{name}::run_{name}() != 0;\n')
out.append('  return failures == 0 ? 0 : 1;\n}\n')
with open(os.path.join(work_dir, 'combined_coverage_driver.cpp'), 'w') as f:
    f.write(''.join(out))
print(f"combined {len(names)} test files", file=sys.stderr)
PYEOF

cd "$repo_root"
g++ -std=c++17 -Iinclude -Itest -fprofile-instr-generate -fcoverage-mapping -g -O0 \
  "$work_dir/combined_coverage_driver.cpp" -o "$work_dir/combined_test"

cd "$work_dir"
LLVM_PROFILE_FILE=combined.profraw ./combined_test > combined_out.txt 2>&1 || true
"${llvm_profdata[@]}" merge -sparse combined.profraw -o combined.profdata

if [[ -n "$1" ]]; then
  "${llvm_cov[@]}" show ./combined_test -instr-profile=combined.profdata \
    "$repo_root/$1" 2>/dev/null
else
  "${llvm_cov[@]}" report ./combined_test -instr-profile=combined.profdata \
    -ignore-filename-regex='(optional_lite|string_view_lite|variant_lite|termcolor|combined_coverage_driver|test_utils)' 2>/dev/null
fi
