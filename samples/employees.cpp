#include <tabulate/table.hpp>
using namespace tabulate;

#if __cplusplus >= 201703L
#include <variant>
using std::variant;
#else
#include <tabulate/variant_lite.hpp>
using nonstd::variant;
#endif
using Row_t = std::vector<variant<std::string, const char *, Table>>;

int main() {
  Table employees;

  Table department;
  department.add_row(Row_t{"Research", "Engineering"});

  // Add rows
  employees.add_row(Row_t{"Emp. ID", "First Name", "Last Name", "Department / Business Unit"});
  employees.add_row(Row_t{"101", "Donald", "Patrick", "Finance"});
  employees.add_row(
      Row_t{"102", "Rachel", "Williams", "Marketing and Operational\nLogistics Planning"});
  employees.add_row(Row_t{"103", "Ian", "Jacob", department});

  employees.column(0)
      .format()
      .font_style({FontStyle::bold})
      .font_color(Color::white)
      .font_align(FontAlign::right);

  employees.column(3).format().font_align(FontAlign::center);

  // Print the table
  std::cout << employees << std::endl;
}