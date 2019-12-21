#include <cstddef>
#include <string>
#include <optional>

namespace tables {

struct Format {
  std::optional<size_t> width{3};
  std::optional<size_t> height{};
  
  size_t margin_left{1};
  size_t margin_top{1};
  size_t margin_right{1};
  size_t margin_bottom{1}; 
  
  size_t padding_left{1};
  size_t padding_top{1};
  size_t padding_right{1};
  size_t padding_bottom{1};

  std::string border_top{"-"};
  std::string border_bottom{"-"};
  std::string border_left{"|"};
  std::string border_right{"|"};  

  std::string corner_top_left{"+"};
  std::string corner_top_right{"+"};
  std::string corner_bottom_left{"+"};
  std::string corner_bottom_right{"+"};  
  
  std::string column_separator{"|"};
};
  
};
