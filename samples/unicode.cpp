#include <tabulate/table.hpp>
using namespace tabulate;

int main() {
  Table unicode;

  unicode.format().corner("♥").font_style({FontStyle::bold});

  unicode.add_row({"English", "I love you"});
  unicode.add_row({"French", "Je t’aime"});
  unicode.add_row({"Spanish", "Te amo"});
  unicode.add_row({"German", "Ich liebe Dich"});
  unicode.add_row({"Mandarin Chinese", "我爱你"});
  unicode.add_row({"Japanese", "愛してる"});
  unicode.add_row({"Korean", "사랑해 (Saranghae)"});
  unicode.add_row({"Greek", "Σ΄αγαπώ (Se agapo)"});
  unicode.add_row({"Italian", "Ti amo"});
  unicode.add_row({"Russian", "Я тебя люблю (Ya tebya liubliu)"});
  unicode.add_row({"Hebrew", "אני אוהב אותך (Ani ohev otakh)"});

  // Set the table-level locale
  unicode.column(0).format().locale("en_US.UTF-8");

  // Set English-US locale for first column
  unicode[0][1].format().locale("en_US.UTF-8");

  // Set locale for individual cells
  unicode[1][1].format().locale("fr_FR.UTF-8");  // French
  unicode[2][1].format().locale("es_ES.UTF-8");  // Spanish
  unicode[3][1].format().locale("de_DE.UTF-8");  // German
  unicode[4][1].format().locale("zh_CN.UTF-8");  // Chinese
  unicode[5][1].format().locale("ja_JP.UTF-8");  // Japanese
  unicode[6][1].format().locale("ko_KR.UTF-8");  // Korean
  unicode[7][1].format().locale("el_GR.UTF-8");  // Greek
  unicode[8][1].format().locale("it_IT.UTF-8");  // Italian
  unicode[9][1].format().locale("ru_RU.UTF-8");  // Russian
  unicode[10][1].format().locale("he_IL.UTF-8"); // Hebrew

  std::cout << unicode << std::endl;
}