#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include <sstream>

#include "test_utils.hpp"

// Coverage for ColumnFormat: every one of its setters forwards the value to
// every cell in the column. Also covers Column::cells()/begin()/end() and
// the multi-line branch of Column::get_cell_width().
int main() {
  tabulate_test::Harness t;
  using tabulate::Color;
  using tabulate::FontAlign;
  using tabulate::FontStyle;
  using Row_t = tabulate::Table::Row_t;

  auto colorized_output = [](tabulate::Table &table) {
    std::stringstream stream;
    stream << termcolor::colorize;
    table.print(stream);
    return stream.str();
  };

  // Structural setters, checked via rendered output.
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().width(6);
    t.expect("ColumnFormat::width", table, "+------+\n| x    |\n+------+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().height(3);
    t.expect("ColumnFormat::height", table, "+---+\n| x |\n|   |\n|   |\n+---+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().padding(2);
    t.expect("ColumnFormat::padding", table,
             "+-----+\n|     |\n|     |\n|  x  |\n|     |\n|     |\n+-----+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().padding_left(3);
    t.expect("ColumnFormat::padding_left", table, "+-----+\n|   x |\n+-----+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().padding_right(3);
    t.expect("ColumnFormat::padding_right", table, "+-----+\n| x   |\n+-----+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().padding_top(1);
    t.expect("ColumnFormat::padding_top", table, "+---+\n|   |\n| x |\n+---+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().padding_bottom(1);
    t.expect("ColumnFormat::padding_bottom", table, "+---+\n| x |\n|   |\n+---+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().border("*");
    t.expect("ColumnFormat::border", table, "+***+\n* x *\n+***+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().border_left("L");
    t.expect("ColumnFormat::border_left", table, "+---+\nL x |\n+---+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().border_right("R");
    t.expect("ColumnFormat::border_right", table, "+---+\n| x R\n+---+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().border_top("T");
    t.expect("ColumnFormat::border_top", table, "+TTT+\n| x |\n+---+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().border_bottom("B");
    t.expect("ColumnFormat::border_bottom", table, "+---+\n| x |\n+BBB+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().corner("*");
    t.expect("ColumnFormat::corner", table, "*---*\n| x |\n*---*");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().column_separator(":");
    // column_separator currently has no effect on rendering (not read by the
    // printer), so this only exercises the forwarding setter itself.
    t.expect("ColumnFormat::column_separator doesn't crash or change output", table,
             "+---+\n| x |\n+---+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"hi"});
    table.column(0).format().width(10).font_align(FontAlign::center);
    t.expect("ColumnFormat::font_align", table, "+----------+\n|    hi    |\n+----------+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"test"});
    table.column(0).format().multi_byte_characters(true);
    t.expect("ColumnFormat::multi_byte_characters(true) doesn't break plain ASCII", table,
             "+------+\n| test |\n+------+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().locale("C");
    t.expect("ColumnFormat::locale doesn't crash or change plain ASCII output", table,
             "+---+\n| x |\n+---+");
  }

  // Color/style setters, checked via the ANSI codes they must emit.
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().font_style({FontStyle::bold});
    t.expect_eq("ColumnFormat::font_style",
                colorized_output(table).find("\033[1m") != std::string::npos ? "bold" : "not-bold",
                "bold");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().font_color(Color::red);
    t.expect_eq("ColumnFormat::font_color",
                colorized_output(table).find("\033[31m") != std::string::npos ? "red" : "not-red",
                "red");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().font_background_color(Color::red);
    t.expect_eq("ColumnFormat::font_background_color",
                colorized_output(table).find("\033[41m") != std::string::npos ? "red" : "not-red",
                "red");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().color(Color::green);
    std::string out = colorized_output(table);
    bool has_font = out.find("\033[32m") != std::string::npos;
    bool has_border = out.rfind("\033[32m") != out.find("\033[32m"); // appears more than once
    t.expect_eq("ColumnFormat::color sets font color", has_font ? "green" : "not-green", "green");
    t.expect_eq("ColumnFormat::color also sets border/corner color",
                has_border ? "multiple" : "single", "multiple");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().background_color(Color::yellow);
    t.expect_eq("ColumnFormat::background_color",
                colorized_output(table).find("\033[43m") != std::string::npos ? "yellow" : "not-yellow",
                "yellow");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().border_color(Color::cyan);
    t.expect_eq("ColumnFormat::border_color",
                colorized_output(table).find("\033[36m") != std::string::npos ? "cyan" : "not-cyan",
                "cyan");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().border_background_color(Color::cyan);
    t.expect_eq("ColumnFormat::border_background_color",
                colorized_output(table).find("\033[46m") != std::string::npos ? "cyan" : "not-cyan",
                "cyan");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().border_left_color(Color::magenta);
    t.expect_eq("ColumnFormat::border_left_color",
                colorized_output(table).find("\033[35m") != std::string::npos ? "magenta" : "not-magenta",
                "magenta");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().border_left_background_color(Color::magenta);
    t.expect_eq(
        "ColumnFormat::border_left_background_color",
        colorized_output(table).find("\033[45m") != std::string::npos ? "magenta" : "not-magenta",
        "magenta");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().border_right_color(Color::blue);
    t.expect_eq("ColumnFormat::border_right_color",
                colorized_output(table).find("\033[34m") != std::string::npos ? "blue" : "not-blue",
                "blue");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().border_right_background_color(Color::blue);
    t.expect_eq("ColumnFormat::border_right_background_color",
                colorized_output(table).find("\033[44m") != std::string::npos ? "blue" : "not-blue",
                "blue");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().border_top_color(Color::white);
    t.expect_eq("ColumnFormat::border_top_color",
                colorized_output(table).find("\033[37m") != std::string::npos ? "white" : "not-white",
                "white");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().border_top_background_color(Color::white);
    t.expect_eq("ColumnFormat::border_top_background_color",
                colorized_output(table).find("\033[47m") != std::string::npos ? "white" : "not-white",
                "white");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().border_bottom_color(Color::grey);
    t.expect_eq("ColumnFormat::border_bottom_color",
                colorized_output(table).find("\033[30m") != std::string::npos ? "grey" : "not-grey",
                "grey");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().border_bottom_background_color(Color::grey);
    t.expect_eq("ColumnFormat::border_bottom_background_color",
                colorized_output(table).find("\033[40m") != std::string::npos ? "grey" : "not-grey",
                "grey");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().corner_color(Color::yellow);
    t.expect_eq("ColumnFormat::corner_color",
                colorized_output(table).find("\033[33m") != std::string::npos ? "yellow" : "not-yellow",
                "yellow");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().corner_background_color(Color::yellow);
    t.expect_eq(
        "ColumnFormat::corner_background_color",
        colorized_output(table).find("\033[43m") != std::string::npos ? "yellow" : "not-yellow",
        "yellow");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().column_separator_color(Color::green);
    // column_separator_color has no visible effect (column_separator itself
    // is never rendered), so this just exercises the forwarding setter.
    t.expect("ColumnFormat::column_separator_color doesn't crash", table, "+---+\n| x |\n+---+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().column_separator_background_color(Color::green);
    t.expect("ColumnFormat::column_separator_background_color doesn't crash", table,
             "+---+\n| x |\n+---+");
  }

  // Column::cells(), begin()/end() range-for, and the multi-line branch of
  // get_cell_width() (a cell with more than one real line, so the widest
  // substring must be used instead of the raw cell size).
  {
    tabulate::Table table;
    table.add_row(Row_t{"ab\ncde"});
    table.add_row(Row_t{"z"});
    // "cde" (3 chars) is the widest line, wider than "z" or the first cell's
    // first line "ab" -- so the whole column should size to fit it.
    t.expect("Column::get_cell_width uses the widest line of multi-line content", table,
             "+-----+\n| ab  |\n| cde |\n+-----+\n| z   |\n+-----+");
  }
  {
    tabulate::Table table;
    table.add_row(Row_t{"a"});
    table.add_row(Row_t{"b"});
    table.add_row(Row_t{"c"});
    auto column = table.column(0);
    t.expect_eq("Column::cells() returns one entry per row", std::to_string(column.cells().size()),
                "3");
    size_t visited = 0;
    for (auto &cell : column)
      ++visited;
    (void)visited;
    t.expect_eq("Column begin()/end() visits every cell in the column",
                std::to_string(visited), "3");
  }

  return t.report();
}
