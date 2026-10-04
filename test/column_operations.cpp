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
    table.add_row(Row_t{"Name", "Age", "Country"});
    table.add_row(Row_t{"Alice", "30", "USA"});
    table.erase_column(1);
    t.expect("erase_column removes a middle column from every row", table,
             "+-------+---------+\n| Name  | Country |\n+-------+---------+\n"
             "| Alice | USA     |\n+-------+---------+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"Name", "Age"});
    table.add_row(Row_t{"Alice", "30"});
    table.erase_column(0);
    t.expect("erase_column removes the first column", table,
             "+-----+\n| Age |\n+-----+\n| 30  |\n+-----+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"Name", "Age"});
    table.add_row(Row_t{"Alice", "30"});
    table.erase_column(1);
    t.expect("erase_column removes the last column", table,
             "+-------+\n| Name  |\n+-------+\n| Alice |\n+-------+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"a", "b", "c"});
    table.erase_column(1);
    t.expect_eq("erase_column shrinks shape() column count",
                std::to_string(table.shape().second > 0 ? table.row(0).size() : 0), "2");
  }

  return t.report();
}
