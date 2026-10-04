#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include <sstream>

#include "test_utils.hpp"

// Regression test for https://github.com/p-ranav/tabulate/issues/62:
// Table::print_row()/print_bottom_border() let rows be streamed to a
// stream as they're added, instead of only printing the whole table once at
// the end. With fixed column widths, streaming row-by-row must produce
// output identical to a normal full print().
int main() {
  tabulate_test::Harness t;
  using Row_t = tabulate::Table::Row_t;

  tabulate::Table table;
  table.add_row(Row_t{"ID", "Status"});
  table.column(0).format().width(6);
  table.column(1).format().width(10);

  std::ostringstream streamed;
  table.print_row(0, streamed);

  table.add_row(Row_t{"1", "OK"});
  table.print_row(1, streamed);

  table.add_row(Row_t{"2", "FAILED"});
  table.print_row(2, streamed);

  table.print_bottom_border(streamed);

  t.expect_eq("streaming rows one at a time matches a normal full print()", streamed.str(),
              table.str() + "\n");

  return t.report();
}
