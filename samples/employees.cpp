#include <tabulate/table.hpp>

using namespace tabulate;

int main() {
  Table employees;

  // Add rows
  employees.add_row({"Emp. ID", "First Name", "Last Name", "Department / Business Unit"});
  employees.add_row({"101", "Donald", "Patrick", "Finance"});
  employees.add_row({"102", "Donald", "Patrick", "Marketing and Operational Logistics Planning"});
  employees.add_row({"103", "Ian", "Jacob", "Engineering"});

  employees.column(0).format()
    .font_align(FontAlign::right);

  // employees.column(3).format()
  //   .width(25);

  // employees[0].format()
  //   // .font_align(FontAlign::right)
  //   .font_style({FontStyle::bold});

  // employees[1][3].format()
  //   .font_align(FontAlign::center);

  // Print the table
  employees.print(std::cout);
}
