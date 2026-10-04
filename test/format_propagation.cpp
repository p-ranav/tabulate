#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include <sstream>

#include "test_utils.hpp"

// Regression tests for https://github.com/p-ranav/tabulate/issues/80:
// changing a format after a table has already been printed must still take
// effect, while explicit row/cell overrides must still win over later
// ancestor changes to the same field.
int main() {
  tabulate_test::Harness t;
  using tabulate::Format;

  {
    tabulate::Table table;
    table.add_row({"hello"});
    table.add_row({"world"});
    (void)table.str(); // force a render, caching Row/Cell formats internally

    table.format().hide_border_bottom();
    t.expect("table-level format change propagates after a print", table,
             "+-------+\n| hello |\n+-------+\n| world |\n");
  }

  {
    tabulate::Table table;
    table.add_row({"hello"});
    (void)table.str();

    table.format().width(9);
    t.expect("width change propagates after a print", table, "+---------+\n| hello   |\n+---------+");
  }

  {
    tabulate::Table table;
    table.add_row({"a"});
    table.add_row({"b"});
    (void)table.str();

    // Row 0 explicitly opts out of a bottom border; row 1 is left alone.
    table[0].format().hide_border_bottom();
    table.format().hide_border_top();
    t.expect("row-level override survives an unrelated later table-level change", table,
             "| a |\n| b |\n+---+");
  }

  {
    tabulate::Table table;
    table.add_row({"a"});
    (void)table.str();
    table[0][0].format().font_color(tabulate::Color::red);
    table.format().font_color(tabulate::Color::blue);
    // Cell-level color should win over the later table-level color, and the
    // actual ANSI output (not just str()) is what encodes that, so compare
    // the colorized stream directly.
    std::stringstream stream;
    stream << termcolor::colorize;
    table.print(stream);
    bool has_red = stream.str().find("\033[31m") != std::string::npos;
    bool has_blue = stream.str().find("\033[34m") != std::string::npos;
    t.expect_eq("cell-level color overrides a later table-level color", has_red ? "red" : "not-red",
                "red");
    t.expect_eq("table-level color change does not clobber a cell override",
                has_blue ? "blue" : "not-blue", "not-blue");
  }

  return t.report();
}
