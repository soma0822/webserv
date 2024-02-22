#include "aparser.hpp"

bool AParser::is_num(const std::string &str){
  for (size_t i = 0; i < str.size(); i++){
    if (!std::isdigit(str[i])){
      return false;
    }
  }
  return true;
}

bool AParser::is_path(const std::string &str){
  if (str[0] != '/'){
    return false;
  }
  return true;
}