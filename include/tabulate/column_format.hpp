#pragma once

namespace tabulate {

class ColumnFormat : public Format {
public:
  ColumnFormat(class Column &column) : column_(column) {}

  Format &width(size_t value);
  Format &height(size_t value);

  // Margin styling
  Format &margin(size_t value);
  Format &margin_left(size_t value);
  Format &margin_right(size_t value);
  Format &margin_top(size_t value);
  Format &margin_bottom(size_t value);

  // Padding
  Format &padding(size_t value);
  Format &padding_left(size_t value);
  Format &padding_right(size_t value);
  Format &padding_top(size_t value);
  Format &padding_bottom(size_t value);

  // Border
  Format &border(const std::string &value);
  Format &border_color(Color value);
  Format &border_background_color(Color value);
  Format &border_left(const std::string &value);
  Format &border_left_color(Color value);
  Format &border_left_background_color(Color value);
  Format &border_right(const std::string &value);
  Format &border_right_color(Color value);
  Format &border_right_background_color(Color value);
  Format &border_top(const std::string &value);
  Format &border_top_color(Color value);
  Format &border_top_background_color(Color value);
  Format &border_bottom(const std::string &value);
  Format &border_bottom_color(Color value);
  Format &border_bottom_background_color(Color value);

  // Corner
  Format &corner(const std::string &value);
  Format &corner_color(Color value);
  Format &corner_background_color(Color value);

  // Column separator
  Format &column_separator(const std::string &value);
  Format &column_separator_color(Color value);
  Format &column_separator_background_color(Color value);

  // Font styling
  Format &font_align(FontAlign value);
  Format &font_style(const std::vector<FontStyle> &style);
  Format &font_color(Color value);
  Format &font_background_color(Color value);
  Format &color(Color value);
  Format &background_color(Color value);

private:
  std::reference_wrapper<class Column> column_;
};

} // namespace tabulate