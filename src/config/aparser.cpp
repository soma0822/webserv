#include "aparser.hpp"

bool AParser::IsNum(const std::string &str) {
  for (size_t i = 0; i < str.size(); i++) {
    if (!std::isdigit(str.at(i))) {
      return false;
    }
  }
  return true;
}

bool AParser::IsPath(const std::string &str) {
  if (str.at(0) != '/') {
    return false;
  }
  return true;
}

int AParser::StrToI(std::string const &str) {
  long long ret = 0;

  for (unsigned long i = 0; i < str.length(); i++) {
    if (str.at(i) >= '0' && str.at(i) <= '9') {
      if (ret > INT_MAX / 10 ||
          (ret == INT_MAX / 10 && str.at(i) > INT_MAX % 10))
        throw std::invalid_argument(str + " is too large");
      ret = ret * 10 + (str.at(i) - '0');
    } else {
      throw std::invalid_argument(str + " is not a number");
    }
  }
  return ret;
}
