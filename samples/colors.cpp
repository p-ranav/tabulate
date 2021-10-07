#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

int main() {
  Table colors;

  colors.add_row(Row_t{"Font Color is Red", "Font Color is Blue", "Font Color is Green"});
  colors.add_row(Row_t{"Everything is Red", "Everything is Blue", "Everything is Green"});
  colors.add_row(
      Row_t{"Font Background is Red", "Font Background is Blue", "Font Background is Green"});

  colors[0][0].format().font_color(Color::red).font_style({FontStyle::bold});
  colors[0][1].format().font_color(Color::blue).font_style({FontStyle::bold});
  colors[0][2].format().font_color(Color::green).font_style({FontStyle::bold});

  colors[1][0]
      .format()
      .border_left_color(Color::red)
      .border_left_background_color(Color::red)
      .font_background_color(Color::red)
      .font_color(Color::red);

  colors[1][1]
      .format()
      .border_left_color(Color::blue)
      .border_left_background_color(Color::blue)
      .font_background_color(Color::blue)
      .font_color(Color::blue);

  colors[1][2]
      .format()
      .border_left_color(Color::green)
      .border_left_background_color(Color::green)
      .font_background_color(Color::green)
      .font_color(Color::green)
      .border_right_color(Color::green)
      .border_right_background_color(Color::green);

  colors[2][0].format().font_background_color(Color::red).font_style({FontStyle::bold});
  colors[2][1].format().font_background_color(Color::blue).font_style({FontStyle::bold});
  colors[2][2].format().font_background_color(Color::green).font_style({FontStyle::bold});

  std::cout << colors << std::endl;
}