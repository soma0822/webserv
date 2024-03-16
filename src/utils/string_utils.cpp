#include "string_utils.hpp"

namespace string_utils {
Result<int, std::string> StrToI(const std::string &str) {
  long long ret = 0;

  if (!validation::IsNumber(str)) {
    return Err(str + "は数値ではありません");
  }
  for (unsigned long i = 0; i < str.length(); ++i) {
    if (ret > INT_MAX / 10 ||
        (ret == INT_MAX / 10 && (str.at(i) - '0') > INT_MAX % 10))
      return Err(str + "はINT_MAXより大きい数値です");
    ret = ret * 10 + (str.at(i) - '0');
  }
  return Ok(ret);
}

std::string SkipSpace(std::string s) {
  const std::string whitespace = " \t\f\v\n\r";
  size_t pos_first = s.find_first_not_of(whitespace);
  size_t pos_last = s.find_last_not_of(whitespace);
  return s.substr(pos_first, pos_last - pos_first + 1);
}
}  // namespace string_utils
