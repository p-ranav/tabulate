#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include "test_utils.hpp"

// Coverage for padding, custom border/corner characters, and the
// hide_border_* family of toggles.
int main() {
  tabulate_test::Harness t;
  using Row_t = tabulate::Table::Row_t;

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().padding_left(3);
    t.expect("padding_left(3) adds only left padding", table, "+-----+\n|   x |\n+-----+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().padding_right(3);
    t.expect("padding_right(3) adds only right padding", table, "+-----+\n| x   |\n+-----+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().padding_top(1).padding_bottom(1);
    t.expect("padding_top(1)+padding_bottom(1) add blank lines", table,
             "+---+\n|   |\n| x |\n|   |\n+---+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().padding(2);
    t.expect("padding(2) sets all four sides at once", table,
             "+-----+\n|     |\n|     |\n|  x  |\n|     |\n|     |\n+-----+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.format()
        .corner_top_left("A")
        .corner_top_right("B")
        .corner_bottom_left("C")
        .corner_bottom_right("D");
    t.expect("each corner can be set independently", table, "A---B\n| x |\nC---D");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.format().border_left("L").border_right("R").border_top("T").border_bottom("B");
    t.expect("each border side can use its own character", table, "+TTT+\nL x R\n+BBB+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.format().hide_border_left();
    t.expect("hide_border_left removes only the left border", table, "+---+\n  x |\n+---+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.format().hide_border_right();
    t.expect("hide_border_right removes only the right border", table, "+---+\n| x  \n+---+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.format().hide_border_top();
    t.expect("hide_border_top removes only the top border", table, "| x |\n+---+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.format().hide_border_bottom();
    t.expect("hide_border_bottom removes only the bottom border", table, "+---+\n| x |\n");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.format().hide_border();
    t.expect("hide_border() removes all four borders and corners", table, "  x  \n");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.format().hide_border();
    table.format().show_border_left();
    t.expect("show_border_left re-enables only the left border", table, "| x  \n");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.format().hide_border();
    table.format().show_border_right();
    t.expect("show_border_right re-enables only the right border", table, "  x |\n");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.format().hide_border();
    table.format().show_border_top();
    t.expect("show_border_top re-enables only the top border", table, "+---+\n  x  \n");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.format().hide_border();
    table.format().show_border_bottom();
    t.expect("show_border_bottom re-enables only the bottom border", table, "  x  \n+---+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.format().hide_border();
    table.format().show_border();
    t.expect("show_border() re-enables all four borders and corners", table,
             "+---+\n| x |\n+---+");
  }

  {
    // The bottom border is skipped entirely when both the corner and the
    // border character are literally empty strings (not merely hidden).
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.format().corner_bottom_left("").corner_bottom_right("").border_bottom("");
    t.expect("an empty corner+border_bottom string skips the bottom border entirely", table,
             "+---+\n| x |");
  }

  return t.report();
}
