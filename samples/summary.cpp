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

  Table empty_row;
  empty_row.format().hide_border();
  readme.add_row({empty_row});
  readme[4].format().hide_border_left().hide_border_right();

  readme.add_row({"Easily format and align content within cells"});
  readme[5].format().font_align(FontAlign::center);

  Table format;
  format.add_row({"Horizontal Alignment", "Left aligned", "Center aligned", "Right aligned"});
  format[0][0].format().font_color(Color::green);
  format.column(1).format().width(25);
  format.column(2).format().width(25).font_align(FontAlign::center);
  format.column(3).format().width(25).font_align(FontAlign::right);

  format.add_row({"Automatic Word-Wrapping", "Long sentences automatically word-wrap based on the width of the column", 
    "Word-wrapping also plays nicely with alignment rules. For instance, this cell is center aligned.",
    "Enforce \ncustom word-wrapping \nby embedding '\\n' \ncharacters in your cell\n content."});
  format[1][2].format().font_align(FontAlign::center);
  format[1][3].format().font_align(FontAlign::right);

  format.column(0).format().width(23);

  readme.add_row({format});

  readme[5].format()
    .border_color(Color::green)
    .font_color(Color::cyan)
    .font_style({FontStyle::underline, FontStyle::bold})
    .padding_top(0)
    .padding_bottom(0);

  readme[6].format()
    .hide_border_top()
    .padding_top(0);

  readme.add_row({empty_row});
  readme[7].format().hide_border_left().hide_border_right();

  Table embedded_table;
  embedded_table.add_row(
      {"You can \neven embed...", 
        Table().add_row(
          {"tables", 
            Table().add_row(
              {"within tables",
                Table().add_row({"within tables",
                  Table().add_row({"within tables.. and so on."})
                })})})});

  readme.add_row({"Nested Representations"});
  readme[8].format()
    .font_align(FontAlign::center);

  readme.add_row({embedded_table});

  readme[9].format()
    .border_color(Color::white)
    .font_color(Color::yellow);

  // Print the table
  std::cout << "\n\n";
  std::cout << termcolor::bold << termcolor::red << " pranav";
  std::cout << termcolor::bold << termcolor::yellow << " ~/dev/tabulate" << termcolor::reset;
  std::cout << termcolor::bold << termcolor::green << " git@master" << std::endl;
  std::cout << termcolor::bold << termcolor::white << " $ ./samples/summary\n\n"; 

  std::cout << readme << "\n\n";

  Table chart;
  chart.format()
    .font_style({FontStyle::bold})
    .font_color(Color::white)
    .padding_left(0)
    .padding_right(0)
    .column_separator("")
    .hide_border();

  for (size_t i = 0; i < 9; ++i) {
    std::vector<std::variant<std::string, Table>> row;
    row.push_back(std::to_string(90 - i * 10));
    for (size_t j = 0; j <= 50; ++j) {
      row.push_back(" ");
    }
    chart.add_row(row);
  }

  std::vector<std::variant<std::string, Table>> row;
  for (int i = 0; i <= 12; ++i) {
    if ((i + 1) % 4 == 0) {
        row.push_back(std::to_string(i + 1));
    } else {
      row.push_back(" ");
    }
  }
  chart.add_row(row);
  chart.add_row({});

  chart.column(0).format()
    .padding_left(1)
    .padding_right(1)
    .border_left(" ");

  for (size_t i = 1; i <= 50; ++i) {
    chart.column(i).format()
      .width(2);
  }

  chart.column(2).format()
    .border_color(Color::white)
    .border_left("|")
    .border_top("-");
  chart.column(2)[8].format().background_color(Color::red);
  chart.column(2)[7].format().background_color(Color::red);

  chart.column(3)[8].format().background_color(Color::yellow);
  chart.column(3)[7].format().background_color(Color::yellow);
  chart.column(3)[6].format().background_color(Color::yellow);

  chart.column(6)[8].format().background_color(Color::red);
  chart.column(6)[7].format().background_color(Color::red);
  chart.column(6)[6].format().background_color(Color::red);
  chart.column(6)[5].format().background_color(Color::red);

  chart.column(7)[8].format().background_color(Color::yellow);
  chart.column(7)[7].format().background_color(Color::yellow);
  chart.column(7)[6].format().background_color(Color::yellow);
  chart.column(7)[5].format().background_color(Color::yellow);
  chart.column(7)[4].format().background_color(Color::yellow);

  chart.column(10)[8].format().background_color(Color::red);
  chart.column(10)[7].format().background_color(Color::red);
  chart.column(10)[6].format().background_color(Color::red);
  chart.column(10)[5].format().background_color(Color::red);
  chart.column(10)[4].format().background_color(Color::red);
  chart.column(10)[3].format().background_color(Color::red);

  chart.column(11)[8].format().background_color(Color::yellow);
  chart.column(11)[7].format().background_color(Color::yellow);
  chart.column(11)[6].format().background_color(Color::yellow);
  chart.column(11)[5].format().background_color(Color::yellow);
  chart.column(11)[4].format().background_color(Color::yellow);
  chart.column(11)[3].format().background_color(Color::yellow);
  chart.column(11)[2].format().background_color(Color::yellow);
  chart.column(11)[1].format().background_color(Color::yellow);

  chart[2][15].format().background_color(Color::red);
  chart[2][16].set_text("Batch 1");
  chart.column(16).format().padding_left(1).width(20);

  chart[4][15].format().background_color(Color::yellow);
  chart[4][16].set_text("Batch 2");

  Table legend;
  legend.format()
    .font_style({FontStyle::bold})
    .font_color(Color::white)    
    .border_top("─")
    .border_bottom("─")
    .border_left("│")
    .border_right("│")
    .column_separator("│")
    .corner("·");
  legend.add_row({"Batch 1", "10", "40", "50", "20", "10", "50"});
  legend.add_row({"Batch 2", "30", "60", "(70)", "50", "40", "30"});

  legend[0].format().font_align(FontAlign::center);
  legend[1].format().font_align(FontAlign::center);

  legend.column(0).format()
    .font_align(FontAlign::right)
    .font_color(Color::green)
    .font_background_color(Color::grey);

  legend.column(2).format()
    .font_color(Color::white)
    .font_background_color(Color::red);

  legend[1][3].format()
    .font_color(Color::yellow);

  chart.column(17).format().width(50);

  chart[3][17].set_text("This is an invisible grid.");
  chart[4][17].set_text("As you can see, cells, rows, and columns");
  chart[5][17].set_text("can be formatted easily.");
  chart[7][17].set_text("This cell is green and italic");
  chart[7][17].format()
    .color(Color::green)
    .font_style({FontStyle::italic});

  chart[8][17].set_text("This one's yellow and right-aligned");
  chart[8][17].format()
    .color(Color::yellow)
    .font_align(FontAlign::right);

  chart[9][17].set_text("This one's on 🔥🔥🔥");

  std::cout << chart;
  std::cout << legend << "\n\n";

}
