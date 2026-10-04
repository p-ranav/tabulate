#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include "test_utils.hpp"

namespace {
// A UTF-8 lead byte announces how many continuation bytes (10xxxxxx) follow.
// If a multi-byte character gets split by a byte-offset-based line wrap, one
// of two things happens: a lead byte ends up with too few/no continuation
// bytes following it, or a continuation byte appears without a lead byte
// before it. Either way, this returns false.
bool is_valid_utf8(const std::string &s) {
  size_t i = 0;
  while (i < s.size()) {
    unsigned char c = static_cast<unsigned char>(s[i]);
    size_t len;
    if ((c & 0x80) == 0x00)
      len = 1;
    else if ((c & 0xE0) == 0xC0)
      len = 2;
    else if ((c & 0xF0) == 0xE0)
      len = 3;
    else if ((c & 0xF8) == 0xF0)
      len = 4;
    else
      return false;
    if (i + len > s.size())
      return false;
    for (size_t k = 1; k < len; ++k) {
      if ((static_cast<unsigned char>(s[i + k]) & 0xC0) != 0x80)
        return false;
    }
    i += len;
  }
  return true;
}
} // namespace

// Regression test for https://github.com/p-ranav/tabulate/issues/127: forcing
// a long multi-byte "word" (no spaces to wrap on, e.g. Chinese text) to split
// across lines used to cut UTF-8 characters in half because the split point
// was computed as a byte offset instead of a display-width offset.
int main() {
  tabulate_test::Harness t;
  using Row_t = tabulate::Table::Row_t;

  tabulate::Table table;
  table.add_row(Row_t{"ID", "Status"});
  table.add_row(
      Row_t{"4", "\u8fd9\u662f\u4e00\u6761\u975e\u5e38\u957f\u7684\u4e2d\u6587\u6ce8\u91ca\uff0c"
                 "\u53ef\u4ee5\u89e3\u6790\u5417\uff1f"});
  table.column(1).format().multi_byte_characters(true).width(20);

  const std::string actual = table.str();
  t.expect_eq("forced line-wrapping never splits a multi-byte character in half",
              is_valid_utf8(actual) ? "valid" : "invalid", "valid");

  return t.report();
}

