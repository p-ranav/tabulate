#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include "test_utils.hpp"

// Regression tests for https://github.com/p-ranav/tabulate/issues/111:
// Table::use_unicode_borders() should produce clean box-drawing borders
// without requiring per-row/per-column manual corner configuration.
int main() {
  tabulate_test::Harness t;
  using Row_t = tabulate::Table::Row_t;

  {
    tabulate::Table table;
    table.add_row(Row_t{"A", "B"});
    table.add_row(Row_t{"1", "2"});
    table.use_unicode_borders();
    t.expect("outer box only: no lines between rows", table,
             "\u250c\u2500\u2500\u2500\u252c\u2500\u2500\u2500\u2510\n"
             "\u2502 A \u2502 B \u2502\n"
             "\u2502 1 \u2502 2 \u2502\n"
             "\u2514\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2518");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"A", "B"});
    table.add_row(Row_t{"1", "2"});
    table.use_unicode_borders(true);
    t.expect("row separators use T-junctions and a cross, not plain corners", table,
             "\u250c\u2500\u2500\u2500\u252c\u2500\u2500\u2500\u2510\n"
             "\u2502 A \u2502 B \u2502\n"
             "\u251c\u2500\u2500\u2500\u253c\u2500\u2500\u2500\u2524\n"
             "\u2502 1 \u2502 2 \u2502\n"
             "\u2514\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2518");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"only"});
    table.use_unicode_borders();
    t.expect("a single cell still gets all four distinct corners", table,
             "\u250c\u2500\u2500\u2500\u2500\u2500\u2500\u2510\n"
             "\u2502 only \u2502\n"
             "\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2518");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"col1", "col2"});
    table.add_row(Row_t{"val1", "val2"});
    table.use_unicode_borders(true, tabulate::BorderStyle::Heavy);
    t.expect("BorderStyle::Heavy uses thick box-drawing glyphs", table,
             "\u250f\u2501\u2501\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2501\u2501"
             "\u2513\n"
             "\u2503 col1 \u2503 col2 \u2503\n"
             "\u2523\u2501\u2501\u2501\u2501\u2501\u2501\u254b\u2501\u2501\u2501\u2501\u2501\u2501"
             "\u252b\n"
             "\u2503 val1 \u2503 val2 \u2503\n"
             "\u2517\u2501\u2501\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u2501\u2501"
             "\u251b");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"col1", "col2"});
    table.add_row(Row_t{"val1", "val2"});
    table.use_unicode_borders(true, tabulate::BorderStyle::Double);
    t.expect("BorderStyle::Double uses double-line box-drawing glyphs", table,
             "\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2566\u2550\u2550\u2550\u2550\u2550\u2550"
             "\u2557\n"
             "\u2551 col1 \u2551 col2 \u2551\n"
             "\u2560\u2550\u2550\u2550\u2550\u2550\u2550\u256c\u2550\u2550\u2550\u2550\u2550\u2550"
             "\u2563\n"
             "\u2551 val1 \u2551 val2 \u2551\n"
             "\u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2569\u2550\u2550\u2550\u2550\u2550\u2550"
             "\u255d");
  }

  return t.report();
}
