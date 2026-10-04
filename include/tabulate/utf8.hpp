
/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#include <algorithm>
#include <cstdint>
#include <string>

#include <clocale>
#include <locale>

#include <cstdlib>
#include <cwchar>
#include <tabulate/termcolor.hpp>
#include <wchar.h>

namespace tabulate {

// Advance past the ANSI escape sequence starting at p, which must point at an
// ESC byte, and return a pointer to the first byte after the sequence. Escape
// sequences occupy no columns on screen, so every width calculation has to be
// able to step over them.
inline const char *skip_ansi_escape_sequence(const char *p) {
  const unsigned char introducer = static_cast<unsigned char>(p[1]);

  // CSI: parameter bytes, then intermediate bytes, then one final byte
  if (introducer == '[') {
    p += 2;
    while (*p != '\0' && static_cast<unsigned char>(*p) >= 0x30 &&
           static_cast<unsigned char>(*p) <= 0x3f)
      ++p;
    while (*p != '\0' && static_cast<unsigned char>(*p) >= 0x20 &&
           static_cast<unsigned char>(*p) <= 0x2f)
      ++p;
    if (*p != '\0')
      ++p;
    return p;
  }

  // OSC, DCS, SOS, PM and APC run until a string terminator; OSC also accepts
  // a BEL as the terminator
  if (introducer == ']' || introducer == 'P' || introducer == 'X' || introducer == '^' ||
      introducer == '_') {
    const bool bel_terminated = introducer == ']';
    p += 2;
    while (*p != '\0') {
      if (bel_terminated && static_cast<unsigned char>(*p) == 0x07)
        return p + 1;
      if (static_cast<unsigned char>(*p) == 0x1b && p[1] == '\\')
        return p + 2;
      ++p;
    }
    return p;
  }

  // Two-character escape sequence
  if (introducer >= 0x40 && introducer <= 0x7e)
    return p + 2;

  return p + 1;
}

#if defined(__unix__) || defined(__unix) || defined(__APPLE__)
inline int get_wcswidth(const std::string &string, const std::string &locale,
                        size_t max_column_width) {
  if (string.size() == 0)
    return 0;

  // The behavior of wcwidth() depends on the LC_CTYPE category of the current
  // locale. Set the current locale based on cell properties before computing
  // width, and put back exactly what was there before.
  const char *previous_locale = std::setlocale(LC_CTYPE, nullptr);
  const std::string saved_locale = previous_locale != nullptr ? previous_locale : "";
  std::setlocale(LC_CTYPE, locale.c_str());

  const char *p = string.c_str();
  std::mbstate_t state = std::mbstate_t();
  size_t characters = 0;
  int result = 0;

  while (*p != '\0' && characters < max_column_width) {
    if (static_cast<unsigned char>(*p) == 0x1b) {
      p = skip_ansi_escape_sequence(p);
      continue;
    }

    wchar_t wide_character;
    size_t length = std::mbrtowc(&wide_character, p, MB_CUR_MAX, &state);

    // Truncated multi-byte character at the end of the string
    if (length == static_cast<size_t>(-2))
      break;

    // Invalid byte, skip it and resynchronize
    if (length == static_cast<size_t>(-1)) {
      state = std::mbstate_t();
      ++p;
      continue;
    }

    if (length == 0)
      length = 1;

    // wcwidth() returns 0 for combining characters and -1 for non-printable
    // ones; neither of those advances the cursor
    const int character_width = wcwidth(wide_character);
    if (character_width > 0)
      result += character_width;

    ++characters;
    p += length;
  }

  if (!saved_locale.empty())
    std::setlocale(LC_CTYPE, saved_locale.c_str());

  return result;
}
#endif

// Number of bytes in text that are not part of an ANSI escape sequence, with
// UTF-8 continuation bytes optionally left out of the count
inline size_t count_bytes_outside_ansi_escapes(const std::string &text,
                                               bool skip_utf8_continuation_bytes) {
  size_t length = 0;
  for (const char *p = text.c_str(); *p != '\0';) {
    if (static_cast<unsigned char>(*p) == 0x1b) {
      p = skip_ansi_escape_sequence(p);
      continue;
    }
    if (!skip_utf8_continuation_bytes || (*p & 0xC0) != 0x80)
      ++length;
    ++p;
  }
  return length;
}

inline size_t get_sequence_length(const std::string &text, const std::string &locale,
                                  bool is_multi_byte_character_support_enabled) {
  if (!is_multi_byte_character_support_enabled)
    return count_bytes_outside_ansi_escapes(text, false);

#if defined(_WIN32) || defined(_WIN64)
  (void)locale; // unused parameter
  return count_bytes_outside_ansi_escapes(text, true);
#elif defined(__unix__) || defined(__unix) || defined(__APPLE__)
  auto result = get_wcswidth(text, locale, text.size());
  if (result >= 0)
    return result;
  else
    return count_bytes_outside_ansi_escapes(text, true);
#endif
}

// Returns the number of bytes from the start of `text` whose rendered width
// does not exceed max_width, without ever splitting a multi-byte character
// (or ANSI escape sequence) in half. Always advances past at least one
// character when text is non-empty, even if that character alone is wider
// than max_width, so callers that force-split long words always make
// progress. Mirrors get_sequence_length()'s width accounting exactly, so a
// prefix of this many bytes is guaranteed to measure as <= max_width.
inline size_t byte_offset_for_width(const std::string &text, const std::string &locale,
                                    bool is_multi_byte_character_support_enabled,
                                    size_t max_width) {
  if (!is_multi_byte_character_support_enabled)
    return (std::min)(max_width, text.size());

#if defined(_WIN32) || defined(_WIN64)
  (void)locale;
  size_t consumed_width = 0;
  bool first_character = true;
  const char *p = text.c_str();
  while (*p != '\0') {
    if (static_cast<unsigned char>(*p) == 0x1b) {
      p = skip_ansi_escape_sequence(p);
      continue;
    }
    const char *character_start = p;
    ++p;
    while (*p != '\0' && (static_cast<unsigned char>(*p) & 0xC0) == 0x80)
      ++p; // UTF-8 continuation byte: part of the same character
    if (!first_character && consumed_width + 1 > max_width)
      return static_cast<size_t>(character_start - text.c_str());
    consumed_width += 1;
    first_character = false;
  }
  return text.size();
#elif defined(__unix__) || defined(__unix) || defined(__APPLE__)
  const char *previous_locale = std::setlocale(LC_CTYPE, nullptr);
  const std::string saved_locale = previous_locale != nullptr ? previous_locale : "";
  std::setlocale(LC_CTYPE, locale.c_str());

  const char *p = text.c_str();
  std::mbstate_t state = std::mbstate_t();
  size_t consumed_width = 0;
  bool first_character = true;
  size_t result = text.size();

  while (*p != '\0') {
    if (static_cast<unsigned char>(*p) == 0x1b) {
      p = skip_ansi_escape_sequence(p);
      continue;
    }

    const char *character_start = p;
    wchar_t wide_character;
    size_t length = std::mbrtowc(&wide_character, p, MB_CUR_MAX, &state);

    if (length == static_cast<size_t>(-2))
      break; // truncated multi-byte character at the end of the string

    if (length == static_cast<size_t>(-1)) {
      state = std::mbstate_t();
      length = 1; // invalid byte: treat as width 1 and resynchronize
    } else if (length == 0) {
      length = 1; // embedded null: treat as a 1-byte character
    }

    int character_width = wcwidth(wide_character);
    if (character_width < 0)
      character_width = 0;

    if (!first_character && consumed_width + static_cast<size_t>(character_width) > max_width) {
      result = static_cast<size_t>(character_start - text.c_str());
      break;
    }

    consumed_width += static_cast<size_t>(character_width);
    first_character = false;
    p = character_start + length;
  }

  if (!saved_locale.empty())
    std::setlocale(LC_CTYPE, saved_locale.c_str());
  return result;
#endif
}

} // namespace tabulate
