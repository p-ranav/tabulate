#pragma once
#include <string>
#include <tabulate/table.hpp>

namespace tabulate {

class Exporter {
public:
  virtual std::string dump(Table &table) = 0;
};

} // namespace tabulate
