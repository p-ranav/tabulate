#include <tabulate/table.hpp>
using namespace tabulate;

int main() {

  Table logo;

  // Add rows
  logo.add_row({"tabulate for Modern C++"});

  // Format the table
  logo.format()
    .font_style({FontStyle::bold})
    .font_align(FontAlign::center)
    .font_color(Color::yellow);

  // Print the table
  std::cout << logo << std::endl;
}
