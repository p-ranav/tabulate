#include <tabulate/table.hpp>
using namespace tabulate;

int main() {

  Table tagline;
  tagline.format()
    .font_align(FontAlign::center)
    .border("")
    .corner("")
    .column_separator("")
    .color(Color::green)
    .font_style({FontStyle::bold});
  tagline.add_row({"tabulate", "is a library for printing aligned and formatted tables"});

  tagline[0][0].format()
    .padding(0)
    .padding_right(1)
    .font_color(Color::yellow)
    .font_style({FontStyle::italic, FontStyle::underline});
  
  tagline[0][1].format()
    .font_align(FontAlign::left)
    .padding_left(0);

  std::cout << tagline << std::endl;

  Table readme;

  readme.format()
    .font_style({FontStyle::bold});
  
  // Add rows
  Table highlights;

  highlights.add_row({"Header-only Library", "Requires C++17", "MIT License"});
  
  readme.add_row({"tabulate", "for Modern C++"});
  readme[0].format().font_align(FontAlign::center);

  readme.add_row({highlights});
  readme[1].format().font_align(FontAlign::center);
  readme.add_row({"Features"});

  Table features;
  features.add_row({"Horizontal Alignment", "Left aligned", "Center aligned", "Right aligned"});
  features.column(1).format().width(25);
  features.column(2).format().width(25).font_align(FontAlign::center);
  features.column(3).format().width(25).font_align(FontAlign::right);
  readme.add_row({features});

  readme[2].format()
    .font_color(Color::cyan)
    .font_style({FontStyle::underline, FontStyle::bold})
    .padding_top(0)
    .padding_bottom(0);

  readme[3].format()
    .corner("")
    .padding_top(0)
    .border_top("");

  // readme[1].format()
  //   .font_color(Color::white)
  //   .font_background_color(Color::grey)
  //   .border_left_background_color(Color::grey)
  //   .border_right_background_color(Color::grey);

  // Print the table
  std::cout << readme << std::endl;
}
