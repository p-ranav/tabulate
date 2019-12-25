#include <tabulate/table.hpp>
using namespace tabulate;

int main() {
  
  Table readme;

  readme.format()
    .border_top("─")
    .border_bottom("─")
    .border_left("│")
    .border_right("│")
    .column_separator("│")
    .corner("·")
    .border_color(Color::yellow)
    .font_style({FontStyle::bold});
    
  readme.add_row({"tabulate for Modern C++"});
  readme[0].format().font_align(FontAlign::center).font_style({FontStyle::bold}).font_color(Color::yellow);

  readme.add_row({"https://github.com/p-ranav/tabulate"});
  readme[1].format()
    .font_align(FontAlign::center)
    .font_style({FontStyle::underline, FontStyle::italic})
    .font_color(Color::white)
    .hide_border_top();

  readme.add_row({"Tabulate is a header-only library for printing aligned, formatted, and colorized tables in Modern C++"});
  readme[2].format().font_style({FontStyle::italic}).font_color(Color::magenta);

  Table highlights;
  highlights.add_row({"Header-only Library", "Requires C++17", "MIT License"});
  readme.add_row({highlights});
  readme[3].format().font_align(FontAlign::center).hide_border_top();
  readme.add_row({"Easily format and align content within cells"});
  readme[4].format().font_align(FontAlign::center);

  Table format;
  format.add_row({"Horizontal Alignment", "Left aligned", "Center aligned", "Right aligned"});
  format[0][0].format().font_color(Color::green);
  format.column(1).format().width(25);
  format.column(2).format().width(25).font_align(FontAlign::center);
  format.column(3).format().width(25).font_align(FontAlign::right);

  format.add_row({"Automatic Word-Wrapping", "Long sentences automatically word-wrap based on the width of the column", 
    "Word-wrapping also plays nicely with alignment rules. For instance, this cell is center aligned.",
    "You can also enforce \ncustom word-wrapping \nby embedding '\\n' \ncharacters in your cell\n content."});
  format[1][2].format().font_align(FontAlign::center);
  format[1][3].format().font_align(FontAlign::right);

  format.column(0).format().width(23);

  readme.add_row({format});

  readme[4].format()
    .font_color(Color::cyan)
    .font_style({FontStyle::underline, FontStyle::bold})
    .padding_top(0)
    .padding_bottom(0);

  readme[5].format()
    .hide_border_top()
    .padding_top(0);

  readme.add_row({"Colorize cells, rows, or even entire tables\nWhat's red and smells like blue paint? ... Red paint!"});
  readme[6].format()
    .font_align(FontAlign::center)
    .border_color(Color::red)
    .corner_color(Color::red)
    .hide_border_top()
    .padding_bottom(0)
    .background_color(Color::red);

  Table embedded_table;
  embedded_table.add_row(
      {"You can \nalso embed...", 
        Table().add_row(
          {"tables", 
            Table().add_row(
              {"within tables",
                Table().add_row({"within tables",
                  Table().add_row({"within tables.. and so on."})
                })})})});

  readme.add_row({embedded_table});

  readme[7].format()
    .hide_border_top()
    .font_color(Color::cyan);

  // Print the table
  std::cout << "\n\n" << readme << "\n\n";
}
