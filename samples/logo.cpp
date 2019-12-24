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
  highlights.add_row({""});
  
  readme.add_row({"tabulate for Modern C++"});
  readme[0].format().font_align(FontAlign::center).font_style({FontStyle::bold, FontStyle::underline}).font_color(Color::yellow);

  readme.add_row({"Tabulate is a header-only library for printing aligned and formatted tables"});
  readme[1].format().font_style({FontStyle::italic});

  readme.add_row({highlights});
  readme[2].format().font_align(FontAlign::center).hide_border_top();
  readme.add_row({"Configure / Format / Align"});
  readme[3].format().font_align(FontAlign::center);

  Table features;
  features.add_row({"Horizontal Alignment", "Left aligned", "Center aligned", "Right aligned"});
  features[0][0].format().font_color(Color::green);
  features.column(1).format().width(25);
  features.column(2).format().width(25).font_align(FontAlign::center);
  features.column(3).format().width(25).font_align(FontAlign::right);
  readme.add_row({features});

  readme[3].format()
    .font_color(Color::cyan)
    .font_style({FontStyle::underline, FontStyle::bold})
    .padding_top(0)
    .padding_bottom(0);

  readme[4].format()
    .hide_border_top()
    .padding_top(0);

  // Print the table
  std::cout << readme << std::endl;
}
