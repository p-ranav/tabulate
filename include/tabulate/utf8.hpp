#pragma once
#include <cstdint>
#include <string>
#include <algorithm>

namespace tabulate {

size_t get_sequence_length(std::string text) {
  return (text.length() - std::count_if(text.begin(), text.end(), [](char c)->bool { return (c & 0xC0) == 0x80; }));
}

} 