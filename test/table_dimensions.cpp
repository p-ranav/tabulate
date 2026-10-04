#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include "test_utils.hpp"

// Regression test for https://github.com/p-ranav/tabulate/issues/107:
// shape() returns the *rendered* character width/height of the printed
// table, not the logical row/column count, which surprised users expecting
// numpy-like semantics. Table::dimensions() gives the actual counts.
int main() {
  tabulate_test::Harness t;
  using Row_t = tabulate::Table::Row_t;

  tabulate::Table table;
  table.add_row(Row_t{"cell"});

  auto dims = table.dimensions();
  t.expect_eq("dimensions() reports 1 row for a single added row", std::to_string(dims.first), "1");
  t.expect_eq("dimensions() reports 1 column for a single-cell row", std::to_string(dims.second),
              "1");

  tabulate::Table multi;
  multi.add_row(Row_t{"a", "b", "c"});
  multi.add_row(Row_t{"d", "e", "f"});
  auto multi_dims = multi.dimensions();
  t.expect_eq("dimensions() reports the actual row count", std::to_string(multi_dims.first), "2");
  t.expect_eq("dimensions() reports the actual column count", std::to_string(multi_dims.second), "3");

  multi.erase_column(0);
  t.expect_eq("dimensions() reflects columns removed via erase_column",
              std::to_string(multi.dimensions().second), "2");

  return t.report();
}
