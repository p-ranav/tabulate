#pragma once

namespace tabulate {

class Printer {
public:
  static void print(TableInternal& table, std::ostream& stream);
};

}