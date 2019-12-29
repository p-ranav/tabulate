#pragma once
#include <algorithm>
#include <cstdint>
#include <string>

#include <clocale>
#include <locale>

#include <cstdlib>
#include <wchar.h>
#include <tabulate/termcolor.hpp>

namespace tabulate {

#if defined(__unix__) || defined(__unix) || defined(__APPLE__)
size_t get_wcswidth(const std::string &string, const std::string &locale, size_t max_column_width) {
  if (string.size() == 0)
    return 0;

  // The behavior of wcswidth() depends on the LC_CTYPE category of the current locale.
  // Set the current locale based on cell properties before computing width
  std::locale::global(std::locale(locale));

  // Convert from narrow std::string to wide string
  wchar_t *wide_string = new wchar_t[string.size()];
  std::mbstowcs(wide_string, string.c_str(), string.size());

  // Compute display width of wide string
  int width = wcswidth(wide_string, max_column_width);
  delete[] wide_string;

  std::locale::global(std::locale(""));

  return (width > 0) ? width : 0 /* returns 0 if width <= 0 */;
}
#endif

size_t get_sequence_length(const std::string &text, const std::string &locale) {
#if defined(_WIN32) || defined(_WIN64)
  return (text.length() -
          std::count_if(text.begin(), text.end(),
                        [](char c) -> bool { return (c & 0xC0) == 0x80; }));
#elif defined(__unix__) || defined(__unix) || defined(__APPLE__)
  auto result = get_wcswidth(text, locale, text.size());
  if (result > 0)
    return result;
  else
    return (text.length() - std::count_if(text.begin(), text.end(),
                                          [](char c) -> bool { return (c & 0xC0) == 0x80; }));
#endif
}

} // namespace tabulate