#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include <sstream>

#include "test_utils.hpp"

// Exhaustive coverage: every tabulate::FontStyle value emits its correct
// ANSI SGR code, no style emits no code, and multiple styles on the same
// cell all show up together.
int main() {
  tabulate_test::Harness t;
  using tabulate::FontStyle;
  using Row_t = tabulate::Table::Row_t;

  auto colorized_output = [](tabulate::Table &table) {
    std::stringstream stream;
    stream << termcolor::colorize;
    table.print(stream);
    return stream.str();
  };

  struct StyleCase {
    FontStyle style;
    const char *name;
    const char *code;
  };

  const StyleCase cases[] = {
      {FontStyle::bold, "bold", "\033[1m"},     {FontStyle::dark, "dark", "\033[2m"},
      {FontStyle::italic, "italic", "\033[3m"}, {FontStyle::underline, "underline", "\033[4m"},
      {FontStyle::blink, "blink", "\033[5m"},   {FontStyle::reverse, "reverse", "\033[7m"},
      {FontStyle::concealed, "concealed", "\033[8m"}, {FontStyle::crossed, "crossed", "\033[9m"},
  };

  for (const auto &c : cases) {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().font_style({c.style});
    bool has_code = colorized_output(table).find(c.code) != std::string::npos;
    std::string label = std::string("font_style({") + c.name + "}) emits its ANSI code";
    t.expect_eq(label.c_str(), has_code ? "present" : "missing", "present");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    std::string out = colorized_output(table);
    bool any_style_code = out.find("\033[1m") != std::string::npos ||
                          out.find("\033[2m") != std::string::npos ||
                          out.find("\033[3m") != std::string::npos ||
                          out.find("\033[4m") != std::string::npos;
    t.expect_eq("no font_style() requested emits no style code",
                any_style_code ? "present" : "missing", "missing");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().font_style({FontStyle::bold, FontStyle::underline});
    std::string out = colorized_output(table);
    bool has_bold = out.find("\033[1m") != std::string::npos;
    bool has_underline = out.find("\033[4m") != std::string::npos;
    t.expect_eq("font_style({bold, underline}) emits bold", has_bold ? "present" : "missing",
                "present");
    t.expect_eq("font_style({bold, underline}) emits underline",
                has_underline ? "present" : "missing", "present");
  }

  {
    // An out-of-range FontStyle (not one of the named enumerators) must be
    // ignored rather than crash -- defensive handling in apply_font_style().
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().font_style({static_cast<FontStyle>(99)});
    std::string out = colorized_output(table);
    t.expect_eq("an out-of-range FontStyle value doesn't crash", out.empty() ? "empty" : "non-empty",
                "non-empty");
  }

  return t.report();
}
