#include <tabulate/table.hpp>

using namespace tabulate;

int main() {
  Table employees;

  // Add rows
  employees.add_row({"Emp. ID", "First Name", "Last Name", "Department / Business Unit"});
  employees.add_row({"101", "Donald", "Patrick", "Finance"});
  employees.add_row({"102", "Rachel", "Williams", "Marketing and \nOperational Logistics\nPlanning"});
  employees.add_row({"103", "Ian", "Jacob", "Engineering"});

  employees.column(0).format()
    .font_style({FontStyle::bold})
    .font_color(Color::white)
    .font_align(FontAlign::right);

  employees.column(3).format()
    .font_align(FontAlign::center);

  // employees.column(3).format()
  //   .width(10);

  // employees[0].format()
  //   // .font_align(FontAlign::right)
  //   .font_style({FontStyle::bold});

  // employees[1][3].format()
  //   .font_align(FontAlign::center);

  // Print the table
  employees.print(std::cout);

}
