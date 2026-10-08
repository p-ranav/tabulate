#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include <sstream>

#include "test_utils.hpp"

// Table -> Row -> Cell format precedence, exercised across many different
// Format fields (not just color, which format_propagation.cpp already
// covers). Each field is checked at all three levels: a table-level value
// applies to every cell, a row-level override wins over the table for that
// row, and a cell-level override wins over both for that cell.
int main() {
  tabulate_test::Harness t;
  using tabulate::Color;
  using tabulate::FontAlign;
  using tabulate::FontStyle;
  using Row_t = tabulate::Table::Row_t;

  auto colorized = [](tabulate::Table &table) {
    std::stringstream stream;
    stream << termcolor::colorize;
    table.print(stream);
    return stream.str();
  };

  // font_color: table -> row -> cell
  {
    tabulate::Table table;
    table.add_row(Row_t{"a"});
    table.add_row(Row_t{"b"});
    table.add_row(Row_t{"c"});
    table.format().font_color(Color::red);
    table[1].format().font_color(Color::green);
    table[2][0].format().font_color(Color::blue);

    std::string out = colorized(table);
    size_t pos_a = out.find('a');
    size_t pos_b = out.find('b');
    size_t pos_c = out.find('c');
    std::string before_a = out.substr(0, pos_a);
    std::string before_b = out.substr(0, pos_b);
    std::string before_c = out.substr(0, pos_c);

    t.expect_eq("font_color: table-level applies to a row with no override",
                before_a.find("\033[31m") != std::string::npos ? "red" : "not-red", "red");
    t.expect_eq("font_color: row-level override wins over the table for that row",
                before_b.find("\033[32m") != std::string::npos ? "green" : "not-green", "green");
    t.expect_eq("font_color: cell-level override wins over the row and table",
                before_c.find("\033[34m") != std::string::npos ? "blue" : "not-blue", "blue");
  }

  // font_style: table -> row -> cell
  {
    tabulate::Table table;
    table.add_row(Row_t{"a"});
    table.add_row(Row_t{"b"});
    table.add_row(Row_t{"c"});
    table.format().font_style({FontStyle::bold});
    table[1].format().font_style({FontStyle::underline});
    table[2][0].format().font_style({FontStyle::italic});

    std::string out = colorized(table);
    size_t pos_a = out.find('a');
    size_t pos_b = out.find('b');
    size_t pos_c = out.find('c');

    t.expect_eq("font_style: table-level applies to a row with no override",
                out.substr(0, pos_a).find("\033[1m") != std::string::npos ? "bold" : "not-bold",
                "bold");
    t.expect_eq("font_style: row-level override wins over the table for that row",
                out.substr(0, pos_b).find("\033[4m") != std::string::npos ? "underline"
                                                                          : "not-underline",
                "underline");
    t.expect_eq("font_style: cell-level override wins over the row and table",
                out.substr(0, pos_c).find("\033[3m") != std::string::npos ? "italic" : "not-italic",
                "italic");
  }

  // padding_left: table -> row -> cell
  {
    tabulate::Table table;
    table.add_row(Row_t{"a"});
    table.add_row(Row_t{"b"});
    table.format().padding_left(3);
    table[1][0].format().padding_left(1);
    t.expect("padding_left: cell-level override wins over the table-level value", table,
             "+-----+\n|   a |\n+-----+\n| b   |\n+-----+");
  }

  // border_color: table -> cell
  {
    tabulate::Table table;
    table.add_row(Row_t{"a"});
    table.add_row(Row_t{"b"});
    table.format().border_color(Color::red);
    table[1][0].format().border_color(Color::green);
    std::string out = colorized(table);
    size_t pos_b = out.find('b');
    t.expect_eq("border_color: cell-level override wins over the table-level value",
                out.substr(0, pos_b).rfind("\033[32m") != std::string::npos ? "green" : "not-green",
                "green");
  }

  // font_align: column -> cell
  {
    tabulate::Table table;
    table.add_row(Row_t{"hi"});
    table.add_row(Row_t{"yo"});
    table.column(0).format().width(10).font_align(FontAlign::right);
    table[1][0].format().font_align(FontAlign::left);
    t.expect("font_align: cell-level override wins over the column-level value", table,
             "+----------+\n|       hi |\n+----------+\n| yo       |\n+----------+");
  }

  // show_row_separator: table-level, applies uniformly (no per-row concept)
  {
    tabulate::Table table;
    table.add_row(Row_t{"a"});
    table.add_row(Row_t{"b"});
    table.format().show_row_separator();
    t.expect("show_row_separator: table-level adds a rule between every row", table,
             "     \n| a |\n+---+\n| b |\n+---+");
  }

  // multi_byte_characters: table -> cell
  {
    tabulate::Table table;
    table.add_row(Row_t{"test"});
    table.format().multi_byte_characters(true);
    t.expect("multi_byte_characters: table-level flag doesn't break plain ASCII", table,
             "+------+\n| test |\n+------+");
  }

  // A format change after printing propagates (regression for the
  // generation-counter cache added to Cell::format()/Row::format()).
  {
    tabulate::Table table;
    table.add_row(Row_t{"a"});
    table.add_row(Row_t{"b"});
    (void)table.str();
    (void)table.str(); // print twice before changing anything: must stay stable
    table.format().font_color(Color::red);
    std::string out = colorized(table);
    size_t pos_a = out.find('a');
    size_t pos_b = out.find('b');
    bool a_red = out.substr(0, pos_a).find("\033[31m") != std::string::npos;
    bool b_red = out.substr(pos_a, pos_b - pos_a).rfind("\033[31m") != std::string::npos;
    t.expect_eq("a table-level change after repeated prints reaches row a", a_red ? "red" : "not-red",
                "red");
    t.expect_eq("a table-level change after repeated prints reaches row b", b_red ? "red" : "not-red",
                "red");
  }

  // Once a cell overrides a field, later unrelated table-level changes must
  // not disturb it, even across several prints.
  {
    tabulate::Table table;
    table.add_row(Row_t{"a"});
    table[0][0].format().font_color(Color::blue);
    (void)table.str();
    table.format().font_color(Color::red);
    (void)table.str();
    table.format().font_background_color(Color::yellow);
    std::string out = colorized(table);
    bool has_blue = out.find("\033[34m") != std::string::npos;
    bool has_red = out.find("\033[31m") != std::string::npos;
    t.expect_eq("cell override survives multiple later unrelated table-level changes",
                has_blue ? "blue" : "not-blue", "blue");
    t.expect_eq("table-level color change never overrides the cell's own color",
                has_red ? "red" : "not-red", "not-red");
  }

  return t.report();
}
