#include <tabulate/table.hpp>
using namespace tabulate;

int main() {
  Table table;

  table.format().corner("♥").font_style({FontStyle::bold});

  table.add_row({"English", "I love you"});
  table.add_row({"French", "Je t’aime"});
  table.add_row({"Spanish", "Te amo"});
  table.add_row({"German", "Ich liebe Dich"});
  table.add_row({"Mandarin Chinese", "我爱你"});
  table.add_row({"Japanese", "愛してる"});
  table.add_row({"Korean", "사랑해 (Saranghae)"});
  table.add_row({"Greek", "Σ΄αγαπώ (Se agapo)"});
  table.add_row({"Italian", "Ti amo"});
  table.add_row({"Russian", "Я тебя люблю (Ya tebya liubliu)"});
  table.add_row({"Hebrew", "אני אוהב אותך (Ani ohev otakh)"});

  // Column 1 is using mult-byte characters
  table.column(1).format().multi_byte_characters(true);

  // Set English-US locale for first column
  table.column(0).format().locale("en_US.UTF-8");
  table[0][1].format().locale("en_US.UTF-8");

  // Set locale for individual cells
  table[1][1].format().locale("fr_FR.UTF-8");  // French
  table[2][1].format().locale("es_ES.UTF-8");  // Spanish
  table[3][1].format().locale("de_DE.UTF-8");  // German
  table[4][1].format().locale("zh_CN.UTF-8");  // Chinese
  table[5][1].format().locale("ja_JP.UTF-8");  // Japanese
  table[6][1].format().locale("ko_KR.UTF-8");  // Korean
  table[7][1].format().locale("el_GR.UTF-8");  // Greek
  table[8][1].format().locale("it_IT.UTF-8");  // Italian
  table[9][1].format().locale("ru_RU.UTF-8");  // Russian
  table[10][1].format().locale("he_IL.UTF-8"); // Hebrew

  std::cout << table << std::endl;
}