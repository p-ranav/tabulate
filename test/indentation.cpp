#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include <sstream>

#include "test_utils.hpp"

// Regression test for https://github.com/p-ranav/tabulate/issues/96:
// Format::indent() prefixes every rendered line with N spaces.
int main() {
  tabulate_test::Harness t;
  using Row_t = tabulate::Table::Row_t;

  {
    tabulate::Table table;
    table.add_row(Row_t{"a"});
    table.add_row(Row_t{"b"});
    t.expect("indent(0) (default) renders exactly like before", table,
             "+---+\n| a |\n+---+\n| b |\n+---+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"a"});
    table.add_row(Row_t{"b"});
    table.format().indent(4);
    t.expect("indent(4) prefixes every line with 4 spaces", table,
             "    +---+\n    | a |\n    +---+\n    | b |\n    +---+");
  }

  {
    // Bottom border hidden: output ends with a trailing newline even before
    // indenting; that trailing newline must survive, without a stray
    // padding-only line being appended after it.
    tabulate::Table table;
    table.add_row(Row_t{"a"});
    table.add_row(Row_t{"b"});
    table.format().indent(2).hide_border_bottom();
    t.expect_eq("indent() preserves a trailing newline from hidden borders exactly",
                table.str(), "  +---+\n  | a |\n  +---+\n  | b |\n");
  }

  for (bool colorized : {false, true}) {
    for (bool hide_bottom_border : {false, true}) {
      tabulate::Table table;
      table.add_row(Row_t{"a\nb", "c"});
      table.format()
          .font_color(tabulate::Color::red)
          .font_background_color(tabulate::Color::blue)
          .font_style({tabulate::FontStyle::bold})
          .border_color(tabulate::Color::green)
          .corner_color(tabulate::Color::yellow);
      if (hide_bottom_border)
        table.format().hide_border_bottom();

      std::ostringstream unindented, indented;
      if (colorized) {
        unindented << termcolor::colorize;
        indented << termcolor::colorize;
      }
      table.print(unindented);
      table.format().indent(3);
      table.print(indented);

      // Indentation adds spaces only; preserve every styling byte and newline.
      const std::string original = unindented.str();
      std::string expected;
      for (size_t i = 0; i < original.size(); ++i) {
        if (i == 0 || original[i - 1] == '\n')
          expected += "   ";
        expected += original[i];
      }
      t.expect_eq("indent() preserves the destination's color mode", indented.str(), expected);
      t.expect_eq("indent() does not change the destination's color mode",
                  std::to_string(termcolor::_internal::is_colorized(indented)),
                  std::to_string(colorized));
      if (!colorized)
        t.expect_eq("plain indented output has no ANSI escapes",
                    indented.str().find('\033') == std::string::npos ? "plain" : "colored",
                    "plain");
    }
  }

  return t.report();
}
