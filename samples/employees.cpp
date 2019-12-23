#include <tabulate/table.hpp>

using namespace tabulate;

int main() {
  Table employees;

  // Add rows
  employees.add_row({"Emp. ID", "First Name", "Last Name", "Department / Business Unit"});
  employees.add_row({"101", "Donald", "Patrick", "Finance"});
  employees.add_row({"102", "Donald", "Patrick", "Marketing and Operational Logistics Planning"});
  employees.add_row({"103", "Ian", "Jacob", "Engineering"});

  employees.format()
    .padding_top(1)
    .padding_bottom(1);

  // employees[1].format()
  //   .width(15);

  // employees[1][2].format()
  //   .width(20);

  // // Print the table
  // employees.print(std::cout);

  // // Set width of column 1 to 13
  // employees.column(1).format()
  //   .width(13);

  // Print the table
  employees.print(std::cout);
}
