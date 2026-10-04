#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include <sstream>

#include "test_utils.hpp"

// Regression tests for https://github.com/p-ranav/tabulate/issues/30:
// font_style previously had no effect on borders or corners.
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
    table.format().border_style({tabulate::FontStyle::bold});
    bool has_bold = colorized_output(table).find("\033[1m") != std::string::npos;
    t.expect_eq("border_style() applies bold to all four borders", has_bold ? "bold" : "not-bold",
                "bold");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.format().corner_style({tabulate::FontStyle::underline});
    bool has_underline = colorized_output(table).find("\033[4m") != std::string::npos;
    t.expect_eq("corner_style() applies underline to all four corners",
                has_underline ? "underline" : "not-underline", "underline");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().border_top_style({tabulate::FontStyle::bold});
    bool has_bold = colorized_output(table).find("\033[1m") != std::string::npos;
    t.expect_eq("border_top_style() can be set independently of other sides",
                has_bold ? "bold" : "not-bold", "bold");
  }

  {
    // Without any style requested, no bold/underline/etc escape codes should
    // be emitted for the border/corner characters.
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    std::string out = colorized_output(table);
    bool has_bold = out.find("\033[1m") != std::string::npos;
    t.expect_eq("default border/corner style emits no style escape codes",
                has_bold ? "bold" : "not-bold", "not-bold");
  }

  return t.report();
}
