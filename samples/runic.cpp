#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

int main() {
  Table table;
  table.format().multi_byte_characters(true);

  /*
    This is a story of a bear and
    a wolf, who wandered the
    realms nine to fulfill a promise
    to one before; they walk the
    twilight path, destined to
    discower the truth
    that is to come.
  */

  table.add_row(Row_t{"ᛏᚺᛁᛊ ᛁᛊ ᚨ ᛊᛏᛟᚱy ᛟᚠᚨ ᛒᛖᚨᚱ ᚨᚾᛞ\n"
                      "ᚨ ᚹᛟᛚᚠ, ᚹᚺᛟ ᚹᚨᚾᛞᛖᚱᛖᛞ ᛏᚺᛖ\n"
                      "ᚱᛖᚨᛚᛗᛊ ᚾᛁᚾᛖ ᛏᛟ ᚠᚢᛚᚠᛁᛚᛚ ᚨ ᛈᚱᛟᛗᛁᛊᛖ\n"
                      "ᛏᛟ ᛟᚾᛖ ᛒᛖᚠᛟᚱᛖ; ᛏᚺᛖy ᚹᚨᛚᚲ ᛏᚺᛖ\n"
                      "ᛏᚹᛁᛚᛁᚷᚺᛏ ᛈᚨᛏᚺ, ᛞᛖᛊᛏᛁᚾᛖᛞ ᛏᛟ\n"
                      "ᛞᛁᛊcᛟᚹᛖᚱ ᛏᚺᛖ ᛏᚱᚢᛏᚺ\nᛏᚺᚨᛏ ᛁᛊ ᛏᛟ cᛟᛗᛖ."});

  table.format()
      .font_style({FontStyle::bold, FontStyle::dark})
      .font_align(FontAlign::center)
      .font_color(Color::red)
      .font_background_color(Color::yellow)
      // Corners
      .corner_top_left("ᛰ")
      .corner_top_right("ᛯ")
      .corner_bottom_left("ᛮ")
      .corner_bottom_right("ᛸ")
      .corner_top_left_color(Color::cyan)
      .corner_top_right_color(Color::yellow)
      .corner_bottom_left_color(Color::green)
      .corner_bottom_right_color(Color::red)
      // Borders
      .border_top("ᛜ")
      .border_bottom("ᛜ")
      .border_left("ᚿ")
      .border_right("ᛆ")
      .border_left_color(Color::yellow)
      .border_right_color(Color::green)
      .border_top_color(Color::cyan)
      .border_bottom_color(Color::red);

  std::cout << table << std::endl;
}