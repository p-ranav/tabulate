# Empty-row rendering regression test

This standalone test uses no test framework. From the repository root, compile
and run it against both distributions:

```sh
c++ -std=c++11 -Wall -Wextra -Iinclude test/empty_rows.cpp -o empty_rows
./empty_rows
c++ -std=c++11 -Wall -Wextra -DTABULATE_SINGLE_HEADER -Isingle_include test/empty_rows.cpp -o empty_rows_single
./empty_rows_single
```

The executable compares complete ASCII output and returns a nonzero exit code
on any mismatch, including in release builds. It covers empty cells and rows,
multiple columns, consecutive rows, vertical and zero padding, configured
height, mixed content, and existing nonempty/newline behavior.
