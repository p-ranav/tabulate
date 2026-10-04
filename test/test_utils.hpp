#pragma once
#include <cstdlib>
#include <iostream>
#include <string>

namespace tabulate_test {

// Shared no-framework harness: compares a table's rendered output against an
// expected ASCII string and tracks pass/fail counts for main() to report.
class Harness {
public:
  template <typename TableT>
  void expect(const char *name, TableT &table, const std::string &expected) {
    ++checks_;
    const std::string actual = table.str();
    if (actual != expected) {
      ++failures_;
      std::cerr << name << "\nExpected:\n" << expected << "\nActual:\n" << actual << '\n';
    }
  }

  void expect_eq(const char *name, const std::string &actual, const std::string &expected) {
    ++checks_;
    if (actual != expected) {
      ++failures_;
      std::cerr << name << "\nExpected: " << expected << "\nActual:   " << actual << '\n';
    }
  }

  int report() const {
    std::cout << checks_ - failures_ << '/' << checks_ << " checks passed\n";
    return failures_ == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
  }

private:
  int checks_ = 0;
  int failures_ = 0;
};

} // namespace tabulate_test
