#include "str_to_i.hpp"

Result<int, std::string> StrToI(const std::string &str) {
  long long ret = 0;

  if (!validation::IsNumber(str)) {
    return Err(str + " is not a number");
  }
  for (unsigned long i = 0; i < str.length(); i++) {
    if (ret > INT_MAX / 10 || (ret == INT_MAX / 10 && (str.at(i) - '0') > INT_MAX % 10))
      return Err(str + " is too large");
    ret = ret * 10 + (str.at(i) - '0');
  }
  return Ok(ret);
}
