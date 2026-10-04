#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include "test_utils.hpp"

int main() {
  tabulate_test::Harness t;
  using tabulate::FontAlign;
  using Row_t = tabulate::Table::Row_t;

  {
    tabulate::Table table;
    table.add_row(Row_t{"hi"});
    table[0][0].format().width(8).font_align(FontAlign::left);
    t.expect("left alignment pads on the right", table, "+--------+\n| hi     |\n+--------+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"hi"});
    table[0][0].format().width(8).font_align(FontAlign::right);
    t.expect("right alignment pads on the left", table, "+--------+\n|     hi |\n+--------+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"hi"});
    table[0][0].format().width(8).font_align(FontAlign::center);
    t.expect("center alignment pads both sides", table, "+--------+\n|   hi   |\n+--------+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"a long sentence that must wrap"});
    table[0][0].format().width(10);
    t.expect("word wrap breaks on word boundaries within the configured width", table,
              "+----------+\n| a long   |\n| sentence |\n| that     |\n| must     |\n| wrap     |"
              "\n+----------+");
  }

  return t.report();
}
