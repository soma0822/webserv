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

int AParser::strtoi(std::string const &str){
    long long ret = 0;

    for (unsigned long i = 0; i < str.length(); i++){
        if (str[i] >= '0' && str[i] <= '9'){
            if (ret > INT_MAX / 10 ||(ret == INT_MAX / 10 && str[i] > INT_MAX % 10))
                throw std::invalid_argument((str + " is too large").c_str());
            ret = ret * 10 + (str[i] - '0');
        } else {
            throw std::invalid_argument((str + " is not a number").c_str());
        }
    }
    return ret;
}
