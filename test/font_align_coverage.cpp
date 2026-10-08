#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include "test_utils.hpp"

// Coverage for font_align at column-, row-, and cell-level, including
// precedence when levels disagree.
int main() {
  tabulate_test::Harness t;
  using tabulate::FontAlign;
  using Row_t = tabulate::Table::Row_t;

  {
    tabulate::Table table;
    table.add_row(Row_t{"hi"});
    table.column(0).format().width(10).font_align(FontAlign::left);
    t.expect("font_align(left) is flush to the left", table, "+----------+\n| hi       |\n+----------+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"hi"});
    table.column(0).format().width(10).font_align(FontAlign::center);
    t.expect("font_align(center) centers the text", table, "+----------+\n|    hi    |\n+----------+");
  }

  {
    // Odd leftover space splits unevenly (one more space after than before).
    tabulate::Table table;
    table.add_row(Row_t{"hi"});
    table.column(0).format().width(9).font_align(FontAlign::center);
    t.expect("font_align(center) with an odd leftover splits unevenly", table,
             "+---------+\n|    hi   |\n+---------+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"hi"});
    table.column(0).format().width(10).font_align(FontAlign::right);
    t.expect("font_align(right) is flush to the right", table, "+----------+\n|       hi |\n+----------+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"hi"});
    t.expect("default font_align is left", table, "+----+\n| hi |\n+----+");
  }

  {
    // Cell-level alignment overrides the column's alignment for that one cell.
    tabulate::Table table;
    table.add_row(Row_t{"hi"});
    table.add_row(Row_t{"world"});
    table.column(0).format().width(10).font_align(FontAlign::right);
    table[0][0].format().font_align(FontAlign::left);
    t.expect("cell-level font_align overrides the column's alignment", table,
             "+----------+\n| hi       |\n+----------+\n|    world |\n+----------+");
  }

  return t.report();
}
