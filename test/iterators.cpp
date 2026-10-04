#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include "test_utils.hpp"

int main() {
  tabulate_test::Harness t;
  using Row_t = tabulate::Table::Row_t;

  {
    tabulate::Table table;
    table.add_row(Row_t{"a"});
    table.add_row(Row_t{"b"});
    table.add_row(Row_t{"c"});
    std::string seen;
    for (auto &row : table)
      seen += row.cell(0).get_text();
    t.expect_eq("range-based for over a Table visits every row in order", seen, "abc");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x", "y", "z"});
    std::string seen;
    for (auto &cell : table[0])
      seen += cell.get_text();
    t.expect_eq("range-based for over a Row visits every cell in order", seen, "xyz");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"h1", "h2"});
    table.add_row(Row_t{"r1", "r2"});
    auto column = table.column(1);
    std::string seen;
    for (size_t i = 0; i < column.size(); ++i)
      seen += column[i].get_text();
    t.expect_eq("table.column(i) visits every cell down that column", seen, "h2r2");
  }

  return t.report();
}
