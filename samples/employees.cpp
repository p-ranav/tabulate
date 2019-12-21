#include <tabulate/table.hpp>

using namespace tabulate;

int main() {
  Table employees;

  // Add rows
  employees.add_row({"Emp. ID", "First Name", "Last Name", "Department"});
  employees.add_row({"101", "Donald", "Patrick", "Finance"});
  employees.add_row({"102", "Donald", "Patrick", "Marketing"});
  employees.add_row({"103", "Ian", "Jacob", "Engineering"});

  // Format the header
  employees[0].format()
    .corner(" ")
    .border(" ")
    .column_separator(" ")
    .color(Color::white)
    .background_color(Color::blue)
    .font_style({FontStyle::bold})
    .padding_top(0)
    .padding_bottom(0);

  // Print the table
  std::cout << employees << std::endl;
}
