#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

// Regression sample for https://github.com/p-ranav/tabulate/issues/127:
// a long run of Chinese text (no spaces to wrap on) forced a word-wrap split,
// which used to cut a multi-byte UTF-8 character in half. Reproduces the
// exact scenario from the issue.
int main() {
  Table table;
  table.add_row(Row_t{"ID", "Status", "Score"});
  table.add_row(Row_t{"1", "\u2714 Passed", "85"});
  table.add_row(Row_t{"2", "\u2718 Failed", "40"});
  table.add_row(Row_t{"3", "\u2714 Passed", "95"});
  table.add_row(Row_t{"4", "\u8fd9\u662f\u4e00\u6761\u975e\u5e38\u957f\u7684\u4e2d\u6587\u6ce8"
                           "\u91ca\uff0c\u53ef\u4ee5\u89e3\u6790\u5417\uff1f",
                      "95"});
  table.add_row(Row_t{"5", "Is there any problem with this very long English comment?", "95"});

  table.column(1).format().multi_byte_characters(true);
  table.column(1).format().width(20);

  table[1][1].format().font_color(Color::green);
  table[2][1].format().font_color(Color::red);
  table[3][1].format().font_color(Color::green);

  std::cout << table << std::endl;
}
