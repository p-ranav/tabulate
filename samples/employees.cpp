#include <tabulate/table.hpp>

using namespace tabulate;

int main() {
  Table employees;

  // Add rows
  employees.add_row({"Emp. ID", "First Name", "Last Name", "Department / Business Unit"});
  employees.add_row({"101", "Donald", "Patrick", "Finance"});
  employees.add_row({"102", "Donald", "Patrick", "Marketing and Operational Logistics Planning"});
  employees.add_row({"103", "Ian", "Jacob", "Engineering"});

  // Print the table
  employees.print(std::cout);
}
