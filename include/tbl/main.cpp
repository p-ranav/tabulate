#include "table.hpp"

using namespace tbl;

int main() {
  { // Default table
    std::cout << "Default table:\n";
    Table table;
    table.add_row({"H1", "H2", "H3"});
    table.add_row({"123", "23456", "3"});
    table.add_row({"4", "5", "678910"});
    std::cout << table << "\n\n";
  }

  { // Right aligned
    std::cout << "Right aligned:\n";
    Table table;
    table.format()
        .font_style({FontStyle::bold})
        .color(Color::white)
        .background_color(Color::green)
        .width(5)
        .font_align(FontAlign::right);
    table.add_row({"H1", "H2", "H3"});
    table.add_row({"123", "23456", "3"});
    table.add_row({"4", "5", "678910"});
    table[1][1].format().color(Color::white).background_color(Color::red);
    std::cout << table << "\n\n";
  }

  { // Custom single row, single col box
    std::cout << "Custom single row, single col box:\n";
    Table box;
    box.add_row({" WARNING "});
    box.format()
        .font_style({FontStyle::bold, FontStyle::blink})
        .color(Color::red)
        .padding_top(0)
        .padding_left(0)
        .padding_right(0)
        .padding_bottom(0)
        .corner("+")
        .border_left("|")
        .border_right("|")
        .border_top("―")
        .border_bottom("―");
    std::cout << box << "\n\n";
  }

  { // Center aligned
    std::cout << "Center aligned:\n";
    Table table;
    table.format().width(5).font_align(FontAlign::center);
    table.add_row({"H1", "H2", "H3"});
    table.add_row({"123", "23456", "3"});
    table.add_row({"4", "5", "678910"});
    std::cout << table << "\n\n";
  }

  { // Row formatting
    std::cout << "Row formatting:\n";
    Table table;
    table.add_row({"H1", "H2", "H3"});
    table.add_row({"123", "23456", "3"});
    table.add_row({"4", "5", "678910"});
    table[0]
        .format()
        .padding_top(0)
        .padding_bottom(0)
        .font_style({FontStyle::bold})
        .font_align(FontAlign::center);
    std::cout << table << "\n\n";
  }

  { // Cell formatting
    std::cout << "Cell formatting:\n";
    Table table;
    table.format().width(6);
    table.add_row({"H1", "H2", "H3"});
    table.add_row({"123", "23456", "3"});
    table.add_row({"4", "5", "678910"});
    table[0][0]
        .format()
        .color(Color::yellow)
        .padding_top(0)
        .padding_bottom(0)
        .font_style({FontStyle::italic, FontStyle::bold})
        .font_align(FontAlign::center);
    std::cout << table << "\n\n";
  }

  { // Single cell in first row
    std::cout << "Single cell in first row:\n";
    Table table;
    table.add_row({"Title"});
    table.add_row({"123", "", "3"});
    table.add_row({"4", "5", "678910"});
    std::cout << table << "\n\n";
  }

  { // No border except row 1 top border
    std::cout << "No border except row 1 top border:\n";
    Table table;
    table.format().column_separator(" ").corner("").border("").padding(0);
    table.add_row({"Title1", "Title2", "Title3"});
    table.add_row({"123", "", "3"});
    table.add_row({"4", "5", "678910"});
    table[0].format().color(Color::green).font_style({FontStyle::bold});
    table[1].format().corner("+").border_top("-").font_align(FontAlign::center);
    std::cout << table << "\n";
  }

  { // Format overriding
    std::cout << "Format overriding:\n";
    Table table;
    table.add_row({"Title1", "Title2", "Title3"});
    table.add_row({"1", "2", "3"});
    table.add_row({"4", "Cell override: 5", "6"});
    // Table-level formatting rules
    table.format()
      .color(Color::white)
      .font_style({FontStyle::bold});
    // Special rule for first row
    table[1].format()
      .color(Color::yellow)
      .font_style({FontStyle::italic});
    table[2].format()
      .padding(0);
    // Special rule for last cell on first row
    table[1][2].format()
      .color(Color::white)
      .background_color(Color::yellow);
    // Special rule for second cell on second row
    table[2][1].format()
      .background_color(Color::red);
    std::cout << table << "\n" << std::endl;
  }

  { // Mario
    std::cout << "Mario:\n";
    Table mario;
    mario.format()
      .color(Color::white)
      .font_style({FontStyle::dark})
      .border("")
      .corner("")
      .column_separator("")
      .padding(0);
    auto rows = 10;
    for (size_t i = 0; i < rows; ++i) {
      std::vector<std::string> row;
      auto cols = 80;
      for (size_t j = 0; j < cols; ++j) {
        row.push_back("█");
      }
      mario.add_row(row);
    }
    // Row 5
    for (size_t i = 1; i < 9; ++i) {
      mario[5][i].format().color(Color::yellow);      
    }     
    for (size_t i = 9; i < 20; ++i) {
      mario[5][i].format().color(Color::red);      
    } 
    for (size_t i = 20; i < 26; ++i) {
      mario[5][i].format().color(Color::yellow);      
    } 
    // Row 6
    for (size_t i = 1; i < 7; ++i) {
      mario[6][i].format().color(Color::yellow);      
    } 
    for (size_t i = 22; i < 26; ++i) {
      mario[6][i].format().color(Color::yellow);      
    } 
    for (size_t i = 7; i < 22; ++i) {
      mario[6][i].format().color(Color::red);      
    }    
    // Row 7
    for (size_t i = 5; i < 12; ++i) {
      mario[7][i].format().color(Color::red);      
    }
    for (size_t i = 15; i < 22; ++i) {
      mario[7][i].format().color(Color::red);      
    }
    // Row 8
    for (size_t i = 3; i < 10; ++i) {
      mario[8][i].format().color(Color::grey);      
    }
    for (size_t i = 17; i < 24; ++i) {
      mario[8][i].format().color(Color::grey);      
    }
    // Row 9
    for (size_t i = 1; i < 10; ++i) {
      mario[9][i].format().color(Color::grey);      
    }
    for (size_t i = 17; i < 26; ++i) {
      mario[9][i].format().color(Color::grey);      
    }
    std::cout << mario;
  }
  return 0;
}
