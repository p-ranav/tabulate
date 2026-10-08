#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include "test_utils.hpp"

// Regression tests for cell text that already carries its own ANSI escape
// sequences (https://github.com/p-ranav/tabulate/issues/127-adjacent: width
// calculations must treat escape sequences as zero-width, in both the
// default and multi_byte_characters(true) code paths), plus a few malformed
// / truncated input edge cases that the width calculators must not crash on.
int main() {
  tabulate_test::Harness t;
  using Row_t = tabulate::Table::Row_t;

  {
    // CSI color codes, default (non-multi-byte) mode: the escape bytes are
    // preserved in the output (so the terminal still renders the color),
    // but must not count toward the column's computed width -- "red" is
    // visually 3 columns wide even though the raw string is much longer.
    tabulate::Table table;
    table.add_row(Row_t{"\x1b[31mred\x1b[0m"});
    t.expect("CSI color codes are zero-width (default mode)", table,
             "+-----+\n| \x1b[31mred\x1b[0m |\n+-----+");
  }

  {
    // Same text, but with multi_byte_characters(true): must give the same
    // visible width via the separate get_wcswidth() code path.
    tabulate::Table table;
    table.add_row(Row_t{"\x1b[31mred\x1b[0m"});
    table.format().multi_byte_characters(true);
    t.expect("CSI color codes are zero-width (multi-byte mode)", table,
             "+-----+\n| \x1b[31mred\x1b[0m |\n+-----+");
  }

  {
    // CJK text combined with an embedded ANSI escape, multi-byte mode: each
    // CJK character is 2 columns wide, so "\xe4\xb8\xad\xe6\x96\x87" is 4 columns, matching the
    // sample this mirrors (ansi_escape_width.cpp).
    tabulate::Table table;
    table.add_row(Row_t{"\x1b[1m\xe4\xb8\xad\xe6\x96\x87\x1b[0m"});
    table.format().multi_byte_characters(true);
    t.expect("CJK width + ANSI escapes combine correctly", table,
             "+------+\n| \x1b[1m\xe4\xb8\xad\xe6\x96\x87\x1b[0m |\n+------+");
  }

  {
    // OSC sequence, BEL-terminated (e.g. a terminal hyperlink), default mode.
    tabulate::Table table;
    table.add_row(Row_t{"\x1b]8;;http://example.com\x07link\x1b]8;;\x07"});
    t.expect("OSC (BEL-terminated) escape sequences are zero-width", table,
             "+------+\n| \x1b]8;;http://example.com\x07link\x1b]8;;\x07 |\n+------+");
  }

  {
    // OSC sequence, string-terminator (ESC \) terminated.
    tabulate::Table table;
    table.add_row(Row_t{"\x1b]8;;http://example.com\x1b\\link\x1b]8;;\x1b\\"});
    t.expect("OSC (ST-terminated) escape sequences are zero-width", table,
             "+------+\n| \x1b]8;;http://example.com\x1b\\link\x1b]8;;\x1b\\ |\n+------+");
  }

  {
    // A two-character escape sequence (not CSI, not OSC): ESC M is "reverse
    // index" and has no parameters, just a single final byte.
    tabulate::Table table;
    table.add_row(Row_t{"\x1bMx"});
    t.expect("a bare two-character escape sequence is zero-width", table, "+---+\n| \x1bMx |\n+---+");
  }

  {
    // An escape sequence truncated at the very end of the string (no final
    // byte at all) must not crash and should still render the visible text.
    tabulate::Table table;
    table.add_row(Row_t{"x\x1b"});
    t.expect("a truncated escape sequence at end of text doesn't crash", table,
             "+---+\n| x\x1b |\n+---+");
  }

  {
    // A lone UTF-8 continuation byte (invalid on its own) in multi-byte
    // mode must be skipped, not crash.
    tabulate::Table table;
    table.add_row(Row_t{"a\x80z"});
    table.format().multi_byte_characters(true);
    const std::string rendered = table.str();
    t.expect_eq("an invalid UTF-8 byte in multi-byte mode doesn't crash",
                rendered.empty() ? "empty" : "non-empty", "non-empty");
  }

  {
    // A multi-byte sequence truncated at the end of the string (e.g. a lone
    // leading byte of a 2-byte UTF-8 character) must not crash.
    tabulate::Table table;
    table.add_row(Row_t{"a\xc3"});
    table.format().multi_byte_characters(true);
    const std::string rendered = table.str();
    t.expect_eq("a truncated multi-byte sequence at end of text doesn't crash",
                rendered.empty() ? "empty" : "non-empty", "non-empty");
  }

  {
    // A CSI sequence with an intermediate byte (0x20-0x2f), not just
    // parameter bytes and a final byte.
    tabulate::Table table;
    table.add_row(Row_t{"\x1b[!phello"});
    t.expect("a CSI sequence with an intermediate byte is zero-width", table,
             "+-------+\n| \x1b[!phello |\n+-------+");
  }

  {
    // An OSC sequence with no terminator at all before the string ends.
    tabulate::Table table;
    table.add_row(Row_t{"\x1b]unterminated"});
    const std::string rendered = table.str();
    t.expect_eq("an unterminated OSC sequence doesn't crash",
                rendered.empty() ? "empty" : "non-empty", "non-empty");
  }

  {
    // Empty cell text in multi-byte mode (get_wcswidth's size()==0 case).
    tabulate::Table table;
    table.add_row(Row_t{""});
    table.format().multi_byte_characters(true);
    t.expect("empty cell text in multi-byte mode doesn't crash", table, "+--+\n|  |\n+--+");
  }

  {
    // A single word far too long to fit in the column forces
    // byte_offset_for_width() to split mid-word, default (non-multi-byte)
    // mode.
    tabulate::Table table;
    table.add_row(Row_t{"\x1b[31msupercalifragilisticexpialidocious\x1b[0m"});
    table.column(0).format().width(10);
    const std::string rendered = table.str();
    t.expect_eq("a too-long word is split mid-word with embedded ANSI codes (default mode)",
                rendered.empty() ? "empty" : "non-empty", "non-empty");
  }

  {
    // Same, but in multi-byte mode, with a malformed byte placed in the
    // MIDDLE of the overlong word (with plenty more content after it so
    // splitting continues past it) so byte_offset_for_width()'s
    // invalid-byte-resync handling actually runs on it, rather than the
    // malformed bytes landing in an untouched tail segment.
    tabulate::Table table;
    table.add_row(
        Row_t{"\x1b[31maaaaaaaaaaaaaaaaaaaa\x80\xc3"
              "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\x1b[0m"});
    table.column(0).format().width(10);
    table.format().multi_byte_characters(true);
    const std::string rendered = table.str();
    t.expect_eq("a too-long word is split mid-word with embedded ANSI codes (multi-byte mode)",
                rendered.empty() ? "empty" : "non-empty", "non-empty");
  }

  {
    // Forced word-wrap splitting text that contains an embedded ANSI code,
    // to exercise byte_offset_for_width()'s ANSI-skip branch too. The color
    // start/end codes stay attached to whichever word they were adjacent to.
    tabulate::Table table;
    table.add_row(Row_t{"\x1b[31mhello world\x1b[0m"});
    table.column(0).format().width(7);
    t.expect("word-wrap splits correctly around embedded ANSI codes", table,
             "+-------+\n| \x1b[31mhello |\n| world\x1b[0m |\n+-------+");
  }

  return t.report();
}
