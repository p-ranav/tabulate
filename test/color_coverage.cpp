#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include <sstream>

#include "test_utils.hpp"

// Exhaustive coverage: every tabulate::Color value emits the correct ANSI
// SGR code for both font_color() and font_background_color(), and
// Color::none emits no code at all (the default, used when colorizing is
// off but could still be requested by the caller).
int main() {
  tabulate_test::Harness t;
  using tabulate::Color;
  using Row_t = tabulate::Table::Row_t;

  auto colorized_output = [](tabulate::Table &table) {
    std::stringstream stream;
    stream << termcolor::colorize;
    table.print(stream);
    return stream.str();
  };

  struct ColorCase {
    Color color;
    const char *name;
    const char *fg_code;
    const char *bg_code;
  };

  // grey and black intentionally share the same ANSI code (see
  // https://github.com/p-ranav/tabulate/issues/94): there is no separate
  // "true black" SGR code distinct from grey in this library's mapping.
  const ColorCase cases[] = {
      {Color::grey, "grey", "\033[30m", "\033[40m"},
      {Color::red, "red", "\033[31m", "\033[41m"},
      {Color::green, "green", "\033[32m", "\033[42m"},
      {Color::yellow, "yellow", "\033[33m", "\033[43m"},
      {Color::blue, "blue", "\033[34m", "\033[44m"},
      {Color::magenta, "magenta", "\033[35m", "\033[45m"},
      {Color::cyan, "cyan", "\033[36m", "\033[46m"},
      {Color::white, "white", "\033[37m", "\033[47m"},
      {Color::black, "black", "\033[30m", "\033[40m"},
  };

  for (const auto &c : cases) {
    {
      tabulate::Table table;
      table.add_row(Row_t{"x"});
      table[0][0].format().font_color(c.color);
      bool has_code = colorized_output(table).find(c.fg_code) != std::string::npos;
      std::string label = std::string("font_color(") + c.name + ") emits its ANSI code";
      t.expect_eq(label.c_str(), has_code ? "present" : "missing", "present");
    }
    {
      tabulate::Table table;
      table.add_row(Row_t{"x"});
      table[0][0].format().font_background_color(c.color);
      bool has_code = colorized_output(table).find(c.bg_code) != std::string::npos;
      std::string label = std::string("font_background_color(") + c.name + ") emits its ANSI code";
      t.expect_eq(label.c_str(), has_code ? "present" : "missing", "present");
    }
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().font_color(Color::none);
    std::string out = colorized_output(table);
    bool any_fg_code = out.find("\033[3") != std::string::npos;
    t.expect_eq("font_color(none) emits no color code", any_fg_code ? "present" : "missing",
                "missing");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().font_background_color(Color::none);
    std::string out = colorized_output(table);
    bool any_bg_code = out.find("\033[4") != std::string::npos;
    t.expect_eq("font_background_color(none) emits no color code", any_bg_code ? "present" : "missing",
                "missing");
  }

  return t.report();
}
