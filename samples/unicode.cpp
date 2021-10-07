#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

int main() {
  Table table;

  table.format()
      .corner("♥")
      .font_style({FontStyle::bold})
      .corner_color(Color::magenta)
      .border_color(Color::magenta);

  table.add_row(Row_t{"English", "I love you"});
  table.add_row(Row_t{"French", "Je t’aime"});
  table.add_row(Row_t{"Spanish", "Te amo"});
  table.add_row(Row_t{"German", "Ich liebe Dich"});
  table.add_row(Row_t{"Mandarin Chinese", "我爱你"});
  table.add_row(Row_t{"Japanese", "愛してる"});
  table.add_row(Row_t{"Korean", "사랑해 (Saranghae)"});
  table.add_row(Row_t{"Greek", "Σ΄αγαπώ (Se agapo)"});
  table.add_row(Row_t{"Italian", "Ti amo"});
  table.add_row(Row_t{"Russian", "Я тебя люблю (Ya tebya liubliu)"});
  table.add_row(Row_t{"Hebrew", "אני אוהב אותך (Ani ohev otakh)"});

  // Column 1 is using mult-byte characters
  table.column(1).format().multi_byte_characters(true);

  std::cout << table << std::endl;
}
