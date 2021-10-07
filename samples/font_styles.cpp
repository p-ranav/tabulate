#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

int main() {
  Table styled_table;
  styled_table.add_row(Row_t{"Bold", "Italic", "Bold & Italic", "Blinking"});
  styled_table.add_row(Row_t{"Underline", "Crossed", "Dark", "Bold, Italic & Underlined"});

  styled_table[0][0].format().font_style({FontStyle::bold});

  styled_table[0][1].format().font_style({FontStyle::italic});

  styled_table[0][2].format().font_style({FontStyle::bold, FontStyle::italic});

  styled_table[0][3].format().font_style({FontStyle::blink});

  styled_table[1][0].format().font_style({FontStyle::underline});

  styled_table[1][1].format().font_style({FontStyle::crossed});

  styled_table[1][2].format().font_style({FontStyle::dark});

  styled_table[1][3].format().font_style(
      {FontStyle::bold, FontStyle::italic, FontStyle::underline});

  std::cout << styled_table << std::endl;
}