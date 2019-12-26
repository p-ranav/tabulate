#include <tabulate/table.hpp>

using namespace tabulate;

int main() {
  Table employees;

  Table department;
  department.add_row({"Research", "Engineering"});

  // Add rows
  employees.add_row({"Emp. ID", "First Name", "Last Name", "Department / Business Unit"});
  employees.add_row({"101", "Donald", "Patrick", "Finance"});
  employees.add_row({"102", "Rachel", "Williams", "Marketing and Operational\nLogistics Planning"});
  employees.add_row({"103", "Ian", "Jacob", department});

  employees.column(0).format()
    .font_style({FontStyle::bold})
    .font_color(Color::white)
    .font_align(FontAlign::right);

  employees.column(3).format()
    .font_align(FontAlign::center);

  // Print the table
  std::cout << employees << std::endl;

}