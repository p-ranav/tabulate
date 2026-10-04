#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include "test_utils.hpp"

// Regression tests for https://github.com/p-ranav/tabulate/issues/56: hiding
// a border must not change a row's width relative to the border/corner lines
// around it, and a nested table's rendered text must not be corrupted by the
// outer cell's default whitespace trimming.
int main() {
  tabulate_test::Harness t;
  using Row_t = tabulate::Table::Row_t;

  {
    tabulate::Table table;
    table.add_row(Row_t{"key", "value1\nvalue2"});
    table[0][0].format().hide_border_left();
    t.expect("hide_border_left keeps every row the same width as the border", table,
             "+-----+--------+\n  key | value1 |\n      | value2 |\n+-----+--------+");
  }

  {
    tabulate::Table inner;
    inner.add_row(Row_t{"key", "value1\nvalue2"});
    inner[0][0].format().hide_border_left();

    tabulate::Table outer;
    outer.add_row(Row_t{inner});
    t.expect("a nested table's hidden-border layout survives re-rendering", outer,
             "+------------------+\n| +-----+--------+ |\n|   key | value1 | |\n"
             "|       | value2 | |\n| +-----+--------+ |\n+------------------+");
  }

  return t.report();
}
