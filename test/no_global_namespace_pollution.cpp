// Regression test: a consumer's own `variant`/`optional`/`string_view` types
// at global scope must not collide with tabulate's internals. Before this
// fix, tabulate.hpp/format.hpp/etc. had `using std::variant;` (and optional,
// string_view, get_if, holds_alternative, visit) at *global* scope, so any
// library that also had its own type with one of these names would fail to
// compile or resolve ambiguously wherever tabulate's headers were included.
template <typename T> struct variant {
  T value;
};

template <typename T> struct optional {
  T value;
};

struct string_view {
  const char *data;
};

#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include "test_utils.hpp"

int main() {
  tabulate_test::Harness t;

  variant<int> user_variant{42};
  optional<int> user_optional{7};
  string_view user_string_view{"hi"};

  tabulate::Table table;
  table.add_row(tabulate::Table::Row_t{"a", "b"});
  t.expect_eq("tabulate still works alongside a consumer's own variant/optional/string_view",
              table.str(), "+---+---+\n| a | b |\n+---+---+");

  t.expect_eq("unqualified `variant` at global scope still resolves to the consumer's own type",
              std::to_string(user_variant.value), "42");
  t.expect_eq("unqualified `optional` at global scope still resolves to the consumer's own type",
              std::to_string(user_optional.value), "7");
  t.expect_eq("unqualified `string_view` at global scope still resolves to the consumer's own type",
              user_string_view.data, "hi");

  return t.report();
}
