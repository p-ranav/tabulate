#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include <sstream>

#include "test_utils.hpp"

// Format::operator==/!= aren't used internally anymore (Cell/Row::format()
// compare a cheap generation counter instead, see table_internal.hpp), but
// they're still public API -- this tests them directly. Also covers the
// std::set_union merge path in Format::merge() for font_style_, which only
// runs when BOTH the cell's own format and its parent's format have an
// explicit font_style (as opposed to one inheriting the other's).
int main() {
  tabulate_test::Harness t;
  using tabulate::Format;
  using tabulate::FontStyle;
  using Row_t = tabulate::Table::Row_t;

  {
    // On a freshly default-constructed Format, font_style_ has no value yet,
    // so font_style() takes its "replace" branch rather than appending to an
    // existing vector.
    Format a;
    a.font_style({FontStyle::bold});
    t.expect_eq("font_style() on a Format with no prior style replaces (doesn't append)",
                a == Format{}.font_style({FontStyle::bold}) ? "equal" : "not-equal", "equal");
  }

  {
    Format a, b;
    t.expect_eq("two default-constructed Formats compare equal", a == b ? "equal" : "not-equal",
                "equal");
    t.expect_eq("two default-constructed Formats are not unequal", a != b ? "unequal" : "not-unequal",
                "not-unequal");
  }

  {
    Format a, b;
    a.width(10);
    t.expect_eq("Formats differing in width() compare unequal", a == b ? "equal" : "not-equal",
                "not-equal");
    t.expect_eq("Formats differing in width() are != ", a != b ? "unequal" : "not-unequal",
                "unequal");
  }

  {
    Format a, b;
    a.width(10);
    b.width(10);
    t.expect_eq("two Formats with the same explicit width() compare equal",
                a == b ? "equal" : "not-equal", "equal");
  }

  {
    Format a, b;
    a.font_color(tabulate::Color::red);
    b.font_color(tabulate::Color::blue);
    t.expect_eq("Formats differing in font_color() compare unequal", a == b ? "equal" : "not-equal",
                "not-equal");
  }

  {
    // Both the cell's own format and its parent's format have an explicit
    // font_style: Format::merge() must union them instead of one replacing
    // the other.
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.format().font_style({FontStyle::bold});
    table[0][0].format().font_style({FontStyle::underline});

    std::stringstream stream;
    stream << termcolor::colorize;
    table.print(stream);
    std::string out = stream.str();
    bool has_bold = out.find("\033[1m") != std::string::npos;
    bool has_underline = out.find("\033[4m") != std::string::npos;
    t.expect_eq("merging a cell's own font_style with the table's unions both: bold",
                has_bold ? "present" : "missing", "present");
    t.expect_eq("merging a cell's own font_style with the table's unions both: underline",
                has_underline ? "present" : "missing", "present");
  }

  {
    // Every field explicitly overridden at the cell level, so a later
    // table-level change forces Format::merge() to take EVERY field from
    // the cell's own (higher-precedence) format rather than the parent's.
    using tabulate::Color;
    using tabulate::FontAlign;
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0]
        .format()
        .width(20)
        .height(3)
        .indent(2)
        .padding_left(2)
        .padding_right(3)
        .padding_top(1)
        .padding_bottom(1)
        .border_left("l")
        .border_left_color(Color::red)
        .border_left_background_color(Color::red)
        .border_left_style({FontStyle::bold})
        .border_top("t")
        .border_top_color(Color::red)
        .border_top_background_color(Color::red)
        .border_top_style({FontStyle::bold})
        .border_bottom("b")
        .border_bottom_color(Color::red)
        .border_bottom_background_color(Color::red)
        .border_bottom_style({FontStyle::bold})
        .border_right("r")
        .border_right_color(Color::red)
        .border_right_background_color(Color::red)
        .border_right_style({FontStyle::bold})
        .corner_top_left("1")
        .corner_top_left_color(Color::red)
        .corner_top_left_background_color(Color::red)
        .corner_top_left_style({FontStyle::bold})
        .corner_top_right("2")
        .corner_top_right_color(Color::red)
        .corner_top_right_background_color(Color::red)
        .corner_top_right_style({FontStyle::bold})
        .corner_bottom_left("3")
        .corner_bottom_left_color(Color::red)
        .corner_bottom_left_background_color(Color::red)
        .corner_bottom_left_style({FontStyle::bold})
        .corner_bottom_right("4")
        .corner_bottom_right_color(Color::red)
        .corner_bottom_right_background_color(Color::red)
        .corner_bottom_right_style({FontStyle::bold})
        .column_separator("s")
        .column_separator_color(Color::red)
        .column_separator_background_color(Color::red)
        .font_align(FontAlign::center)
        .font_style({FontStyle::bold})
        .font_color(Color::red)
        .font_background_color(Color::red)
        .multi_byte_characters(true)
        .locale("C")
        .trim_mode(Format::TrimMode::kNone)
        .show_row_separator()
        // show_row_separator() also sets show_border_top_ = true as a side
        // effect, so these must come after it to keep their own values.
        .hide_border_top()
        .hide_border_bottom()
        .hide_border_left()
        .hide_border_right();
    (void)table.str(); // first touch: direct copy, merge() not involved yet

    // Trigger a re-merge: the table's format changing bumps its generation,
    // so the next format() call must re-run Format::merge(), now with the
    // cell's own format (every field set above) as the higher-precedence
    // "first" argument.
    table.format().indent(0);
    const std::string rendered = table.str();
    t.expect_eq("a table-level change after every field is cell-overridden doesn't crash",
                rendered.empty() ? "empty" : "non-empty", "non-empty");
  }

  return t.report();
}
