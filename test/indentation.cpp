#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include "test_utils.hpp"

// Regression test for https://github.com/p-ranav/tabulate/issues/96:
// Format::indent() prefixes every rendered line with N spaces.
int main() {
  tabulate_test::Harness t;
  using Row_t = tabulate::Table::Row_t;

  {
    tabulate::Table table;
    table.add_row(Row_t{"a"});
    table.add_row(Row_t{"b"});
    t.expect("indent(0) (default) renders exactly like before", table,
             "+---+\n| a |\n+---+\n| b |\n+---+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"a"});
    table.add_row(Row_t{"b"});
    table.format().indent(4);
    t.expect("indent(4) prefixes every line with 4 spaces", table,
             "    +---+\n    | a |\n    +---+\n    | b |\n    +---+");
  }

  {
    // Bottom border hidden: output ends with a trailing newline even before
    // indenting; that trailing newline must survive, without a stray
    // padding-only line being appended after it.
    tabulate::Table table;
    table.add_row(Row_t{"a"});
    table.add_row(Row_t{"b"});
    table.format().indent(2).hide_border_bottom();
    t.expect_eq("indent() preserves a trailing newline from hidden borders exactly",
                table.str(), "  +---+\n  | a |\n  +---+\n  | b |\n");
  }

  return t.report();
}
