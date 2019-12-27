#include <tabulate/table.hpp>
using namespace tabulate;

int main() {
    Table unicode;
    unicode.format()
      .corner("♥")
      .font_style({FontStyle::bold});

    unicode.add_row({"English", "I love you"});
    unicode.add_row({"French", "Je t’aime"});
    unicode.add_row({"Spanish", "Te amo"});
    unicode.add_row({"German", "Ich liebe Dich"});
    unicode.add_row({"Mandarin Chinese", "我爱你 (Wo ai ni)"});
    unicode.add_row({"Japanese", "愛してる (Aishiteru)"});
    unicode.add_row({"Korean", "사랑해 (Saranghae)"});
    unicode.add_row({"Arabic", "ٲنَا بحِبَّك (Ana bahebak)"});
    unicode.add_row({"Hindi", "मैं तुमसे प्यार करता हुँ (Main tumse pyar kartha hoon)"});
    unicode.add_row({"Greek", "Σ΄αγαπώ (Se agapo)"});
    unicode.add_row({"Italian", "Ti amo"});
    unicode.add_row({"Russian", "Я тебя люблю (Ya tebya liubliu)"});
    unicode.add_row({"Hebrew", "אני אוהב אותך (Ani ohev otakh)"});
    unicode.add_row({"Cheyenne", "Nemehotatse"});
    unicode.add_row({"Tagalog", "Mahal kita"});
    unicode.add_row({"Inuktitut", "ᓇᒡᓕᒋᕙᒋᑦ (Nagligivaget)"});

    std::string foo = "我爱你 (Wo ai ni)";
    std::cout << tabulate::get_sequence_length(foo) << std::endl;

    foo = " ٲنَا بحِبَّك (Ana bahebak)";
    std::cout << tabulate::get_sequence_length(foo) << std::endl;

    std::cout << unicode << std::endl;
}