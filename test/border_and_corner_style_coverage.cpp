#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include <sstream>

#include "test_utils.hpp"

// Exhaustive coverage: every FontStyle value applied via border_style() and
// corner_style() emits its correct ANSI code on the border/corner
// characters (border_corner_styles.cpp only covers bold and underline).
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
    {
      tabulate::Table table;
      table.add_row(Row_t{"x"});
      table.format().border_style({c.style});
      bool has_code = colorized_output(table).find(c.code) != std::string::npos;
      std::string label = std::string("border_style({") + c.name + "}) emits its ANSI code";
      t.expect_eq(label.c_str(), has_code ? "present" : "missing", "present");
    }
    {
      tabulate::Table table;
      table.add_row(Row_t{"x"});
      table.format().corner_style({c.style});
      bool has_code = colorized_output(table).find(c.code) != std::string::npos;
      std::string label = std::string("corner_style({") + c.name + "}) emits its ANSI code";
      t.expect_eq(label.c_str(), has_code ? "present" : "missing", "present");
    }
  }

  // Individual position setters (border_style()/corner_style() above only
  // exercise the composite "all four sides/corners at once" setters).
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().border_left_style({FontStyle::bold});
    t.expect_eq("border_left_style() emits bold",
                colorized_output(table).find("\033[1m") != std::string::npos ? "present" : "missing",
                "present");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().border_right_style({FontStyle::bold});
    t.expect_eq("border_right_style() emits bold",
                colorized_output(table).find("\033[1m") != std::string::npos ? "present" : "missing",
                "present");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().border_bottom_style({FontStyle::bold});
    t.expect_eq("border_bottom_style() emits bold",
                colorized_output(table).find("\033[1m") != std::string::npos ? "present" : "missing",
                "present");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().corner_top_left_style({FontStyle::underline});
    t.expect_eq(
        "corner_top_left_style() emits underline",
        colorized_output(table).find("\033[4m") != std::string::npos ? "present" : "missing",
        "present");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().corner_top_right_style({FontStyle::underline});
    t.expect_eq(
        "corner_top_right_style() emits underline",
        colorized_output(table).find("\033[4m") != std::string::npos ? "present" : "missing",
        "present");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().corner_bottom_left_style({FontStyle::underline});
    t.expect_eq(
        "corner_bottom_left_style() emits underline",
        colorized_output(table).find("\033[4m") != std::string::npos ? "present" : "missing",
        "present");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().corner_bottom_right_style({FontStyle::underline});
    t.expect_eq(
        "corner_bottom_right_style() emits underline",
        colorized_output(table).find("\033[4m") != std::string::npos ? "present" : "missing",
        "present");
  }

  return t.report();
}
