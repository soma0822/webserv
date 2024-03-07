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
  size_t pos = s.find_first_not_of(" ");
  s = s.substr(pos);
  return s;
}
}  // namespace string_utils
