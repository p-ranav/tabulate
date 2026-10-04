#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include <sstream>

#include "test_utils.hpp"

// Regression test for https://github.com/p-ranav/tabulate/issues/94: there
// was no Color::black, so writing explicit black text/background was only
// possible via the misleadingly-named Color::grey (which already emitted
// ANSI code 30/40, i.e. true black).
int main() {
  tabulate_test::Harness t;
  using Row_t = tabulate::Table::Row_t;

  auto colorized_output = [](tabulate::Table &table) {
    std::stringstream stream;
    stream << termcolor::colorize;
    table.print(stream);
    return stream.str();
  };

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().font_color(tabulate::Color::black);
    bool has_black_fg = colorized_output(table).find("\033[30m") != std::string::npos;
    t.expect_eq("Color::black emits ANSI foreground code 30", has_black_fg ? "black" : "not-black",
                "black");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().font_background_color(tabulate::Color::black);
    bool has_black_bg = colorized_output(table).find("\033[40m") != std::string::npos;
    t.expect_eq("Color::black emits ANSI background code 40", has_black_bg ? "black" : "not-black",
                "black");
  }

  return t.report();
}
