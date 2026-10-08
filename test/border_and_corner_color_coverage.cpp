#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include <sstream>

#include "test_utils.hpp"

// border_color()/corner_color() and their _background_color() counterparts
// emit the correct ANSI codes (black_color.cpp and color_coverage.cpp only
// cover font_color()/font_background_color()).
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

  const ColorCase cases[] = {
      {Color::red, "red", "\033[31m", "\033[41m"},
      {Color::green, "green", "\033[32m", "\033[42m"},
      {Color::blue, "blue", "\033[34m", "\033[44m"},
      {Color::cyan, "cyan", "\033[36m", "\033[46m"},
  };

  for (const auto &c : cases) {
    {
      tabulate::Table table;
      table.add_row(Row_t{"x"});
      table.format().border_color(c.color);
      bool has_code = colorized_output(table).find(c.fg_code) != std::string::npos;
      std::string label = std::string("border_color(") + c.name + ") emits its ANSI code";
      t.expect_eq(label.c_str(), has_code ? "present" : "missing", "present");
    }
    {
      tabulate::Table table;
      table.add_row(Row_t{"x"});
      table.format().corner_color(c.color);
      bool has_code = colorized_output(table).find(c.fg_code) != std::string::npos;
      std::string label = std::string("corner_color(") + c.name + ") emits its ANSI code";
      t.expect_eq(label.c_str(), has_code ? "present" : "missing", "present");
    }
  }

  for (const auto &c : {ColorCase{Color::yellow, "yellow", "\033[33m", "\033[43m"},
                        ColorCase{Color::magenta, "magenta", "\033[35m", "\033[45m"}}) {
    {
      tabulate::Table table;
      table.add_row(Row_t{"x"});
      table.format().border_background_color(c.color);
      bool has_code = colorized_output(table).find(c.bg_code) != std::string::npos;
      std::string label = std::string("border_background_color(") + c.name + ") emits its ANSI code";
      t.expect_eq(label.c_str(), has_code ? "present" : "missing", "present");
    }
    {
      tabulate::Table table;
      table.add_row(Row_t{"x"});
      table.format().corner_background_color(c.color);
      bool has_code = colorized_output(table).find(c.bg_code) != std::string::npos;
      std::string label = std::string("corner_background_color(") + c.name + ") emits its ANSI code";
      t.expect_eq(label.c_str(), has_code ? "present" : "missing", "present");
    }
  }

  // Individual corner-position color setters (corner_color()/
  // corner_background_color() above only cover all four corners at once).
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().corner_top_left_color(Color::red);
    t.expect_eq("corner_top_left_color() emits its ANSI code",
                colorized_output(table).find("\033[31m") != std::string::npos ? "present" : "missing",
                "present");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().corner_top_left_background_color(Color::red);
    t.expect_eq(
        "corner_top_left_background_color() emits its ANSI code",
        colorized_output(table).find("\033[41m") != std::string::npos ? "present" : "missing",
        "present");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().corner_top_right_color(Color::green);
    t.expect_eq("corner_top_right_color() emits its ANSI code",
                colorized_output(table).find("\033[32m") != std::string::npos ? "present" : "missing",
                "present");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().corner_top_right_background_color(Color::green);
    t.expect_eq(
        "corner_top_right_background_color() emits its ANSI code",
        colorized_output(table).find("\033[42m") != std::string::npos ? "present" : "missing",
        "present");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().corner_bottom_left_color(Color::blue);
    t.expect_eq("corner_bottom_left_color() emits its ANSI code",
                colorized_output(table).find("\033[34m") != std::string::npos ? "present" : "missing",
                "present");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().corner_bottom_left_background_color(Color::blue);
    t.expect_eq(
        "corner_bottom_left_background_color() emits its ANSI code",
        colorized_output(table).find("\033[44m") != std::string::npos ? "present" : "missing",
        "present");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().corner_bottom_right_color(Color::cyan);
    t.expect_eq("corner_bottom_right_color() emits its ANSI code",
                colorized_output(table).find("\033[36m") != std::string::npos ? "present" : "missing",
                "present");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().corner_bottom_right_background_color(Color::cyan);
    t.expect_eq(
        "corner_bottom_right_background_color() emits its ANSI code",
        colorized_output(table).find("\033[46m") != std::string::npos ? "present" : "missing",
        "present");
  }

  return t.report();
}
