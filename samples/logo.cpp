#include <tabulate/table.hpp>
using namespace tabulate;

int main() {

  Table header;
  header.add_row({"tabulate", "for Modern C++"});

  // Format the table
  header[0][0].format()
    .font_style({FontStyle::bold, FontStyle::italic})
    .font_align(FontAlign::center)
    .font_color(Color::yellow)
    .border("")
    .corner("")
    .padding_left(17)
    .column_separator("");

  header[0][1].format()
    .padding(0)
    .border("")
    .corner("")
    .column_separator("")
    .font_color(Color::green)
    .font_style({FontStyle::bold});

  std::cout << header << std::endl;

  Table tagline;
  tagline.format()
    .border("")
    .corner("")
    .column_separator("")
    .color(Color::red)
    .font_style({FontStyle::bold});
  tagline.add_row({"tabulate", " is a library for printing aligned and formatted tabes"});
  tagline[0][0].format()
    .padding(0)
    .font_style({FontStyle::italic, FontStyle::underline});
  tagline[0][1].format()
    .padding_left(0);

  std::cout << tagline << std::endl;

  Table logo;

  logo.format()
    .font_style({FontStyle::bold});
  
  // Add rows
  logo.add_row({"Header-only Library"});
  logo.add_row({"Requires C++17"});
  logo.add_row({"MIT License"});

  logo[1].format()
    .font_color(Color::white)
    .font_background_color(Color::grey)
    .border_left_background_color(Color::grey)
    .border_right_background_color(Color::grey);

  // Print the table
  std::cout << logo << std::endl;
}
