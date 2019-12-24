#include <tabulate/table.hpp>
using namespace tabulate;

int main() {
  
  Table readme;

  readme.format()
    .font_style({FontStyle::bold});
  
  // Add rows
  Table highlights;

  highlights.add_row({"Header-only Library", "Requires C++17", "MIT License"});
  
  readme.add_row({"tabulate for Modern C++"});
  readme[0].format().font_align(FontAlign::center).font_style({FontStyle::bold}).font_color(Color::yellow);
  readme.add_row({"https://github.com/p-ranav/tabulate"});
  readme[1].format().font_align(FontAlign::center).font_style({FontStyle::bold, FontStyle::italic}).font_color(Color::white).hide_border_top();

  readme.add_row({"Tabulate is a header-only library for printing aligned, formatted, and colorized tables in Modern C++"});
  readme[2].format().font_style({FontStyle::italic}).font_color(Color::magenta);

  readme.add_row({highlights});
  readme[3].format().font_align(FontAlign::center).hide_border_top();
  readme.add_row({"Configure / Format / Align"});
  readme[4].format().font_align(FontAlign::center);

  Table features;
  features.add_row({"Horizontal Alignment", "Left aligned", "Center aligned", "Right aligned"});
  features[0][0].format().font_color(Color::green);
  features.column(1).format().width(25);
  features.column(2).format().width(25).font_align(FontAlign::center);
  features.column(3).format().width(25).font_align(FontAlign::right);

  features.add_row({"Automatic Word-Wrapping", "Long sentences automatically word-wrap based on the width of the column", 
    "Word-wrapping also plays nicely with alignment rules. For instance, this cell is center aligned.",
    "You can also enforce \ncustom word-wrapping \nby embedding '\\n' \ncharacters in your text"});
  features[1][2].format().font_align(FontAlign::center);
  features[1][3].format().font_align(FontAlign::right);

  readme.add_row({features});

  readme[4].format()
    .font_color(Color::cyan)
    .font_style({FontStyle::underline, FontStyle::bold})
    .padding_top(0)
    .padding_bottom(0);

  readme[5].format()
    .hide_border_top()
    .padding_top(0);

  // Print the table
  std::cout << termcolor::colorize;
  std::cout << readme << std::endl;
  std::cout << termcolor::nocolorize;
}
