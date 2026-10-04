# Tests

Build and run via CMake/CTest from the repository root (see the "Running Tests"
section of the main [README](../README.md#running-tests)):

```sh
mkdir build && cd build
cmake -Dtabulate_BUILD_TESTS=ON ..
make
ctest --output-on-failure
```

None of these use a test framework: each `.cpp` is a standalone executable
(shared harness in `test_utils.hpp`) that renders a `Table` and compares the
exact ASCII output against an expected string, returning a nonzero exit code
on any mismatch. Every test is built twice by CMake/CTest, once against the
modular headers and once against the generated `single_include` header, so
regressions can't slip into one distribution without the other.

You can also compile and run any of them directly without CMake:

```sh
c++ -std=c++11 -Wall -Wextra -Iinclude test/empty_rows.cpp -o empty_rows
./empty_rows
c++ -std=c++11 -Wall -Wextra -DTABULATE_SINGLE_HEADER -Isingle_include test/empty_rows.cpp -o empty_rows_single
./empty_rows_single
```

## What's covered

*   `empty_rows.cpp` — all-empty rows/cells still render borders, padding,
    explicit height, and mixed empty/nonempty content ([#123](https://github.com/p-ranav/tabulate/issues/123)).
*   `format_propagation.cpp` — changing `table.format()` after a table has
    already been printed takes effect, while explicit row/cell-level overrides
    still take precedence over later ancestor changes ([#80](https://github.com/p-ranav/tabulate/issues/80)).
*   `nested_tables.cpp` — hidden borders keep consistent row width, and a
    nested table's pre-rendered text survives re-rendering in an outer cell
    ([#56](https://github.com/p-ranav/tabulate/issues/56)).
*   `column_operations.cpp` — `Table::erase_column` removes the right column
    (first/middle/last) from every row.
*   `alignment_and_wrapping.cpp` — left/center/right font alignment and
    automatic word wrapping within a configured width.
*   `trim_mode.cpp` — `Format::TrimMode` (`kBoth`/`kNone`/`kLeft`/`kRight`)
    controls per-line whitespace trimming, needed to preserve indentation in
    multi-line content like pretty-printed JSON ([#86](https://github.com/p-ranav/tabulate/issues/86)).
*   `iterators.cpp` — range-based iteration over a `Table`'s rows, a `Row`'s
    cells, and a `Column`'s cells.

