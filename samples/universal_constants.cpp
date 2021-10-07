#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

int main() {

  Table universal_constants;

  universal_constants.add_row(Row_t{"Quantity", "Value"});
  universal_constants.add_row(Row_t{"Characteristic impedance of vacuum", "376.730 313 461... Ω"});
  universal_constants.add_row(
      {"Electric constant (permittivity of free space)", "8.854 187 817... × 10⁻¹²F·m⁻¹"});
  universal_constants.add_row(Row_t{"Magnetic constant (permeability of free space)",
                                    "4π × 10⁻⁷ N·A⁻² = 1.2566 370 614... × 10⁻⁶ N·A⁻²"});
  universal_constants.add_row(Row_t{"Gravitational constant (Newtonian constant of gravitation)",
                                    "6.6742(10) × 10⁻¹¹m³·kg⁻¹·s⁻²"});
  universal_constants.add_row(Row_t{"Planck's constant", "6.626 0693(11) × 10⁻³⁴ J·s"});
  universal_constants.add_row(Row_t{"Dirac's constant", "1.054 571 68(18) × 10⁻³⁴ J·s"});
  universal_constants.add_row(Row_t{"Speed of light in vacuum", "299 792 458 m·s⁻¹"});

  // Table.format() will apply styles for the whole table
  universal_constants.format()
      .font_style({FontStyle::bold})
      .border_top(" ")
      .border_bottom(" ")
      .border_left(" ")
      .border_right(" ")
      .corner(" ");

  // Table[index] is used to access a particular row
  // You can use this to access and format a specific row
  universal_constants[0]
      .format()
      .padding_top(1)
      .padding_bottom(1)
      .font_align(FontAlign::center)
      .font_style({FontStyle::underline})
      .font_background_color(Color::red);

  // Table.column(index) is used to access a particular column
  // You can use this to access and format a specific column
  universal_constants.column(1).format().font_color(Color::yellow);

  universal_constants[0][1].format().font_background_color(Color::blue).font_color(Color::white);

  std::cout << universal_constants << std::endl;
}