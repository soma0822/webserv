#include "string_utils.hpp"

#include <algorithm>

#include "validation.hpp"

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

Result<int, std::string> StrToHex(const std::string &str) {
  long long ret = 0;

  if (!validation::IsHexNumber(str)) {
    return Err(str + "は数値ではありません");
  }
  for (unsigned long i = 0; i < str.length(); ++i) {
    ret = ret * 16;
    if ('0' <= str[i] && str[i] <= '9')
      ret = ret + (str[i] - '0');
    else if ('a' <= str[i] && str[i] <= 'f')
      ret = ret + (str[i] - 'a' + 10);
    else if ('A' <= str[i] && str[i] <= 'F')
      ret = ret + (str[i] - 'A' + 10);
    if (ret > INT_MAX) return Err(str + "はINT_MAXより大きい数値です");
  }
  return Ok(ret);
}

std::string SkipSpace(std::string s) {
  const std::string whitespace = " \t\f\v\n\r";
  size_t pos_first = s.find_first_not_of(whitespace);
  size_t pos_last = s.find_last_not_of(whitespace);
  if (pos_first == std::string::npos || pos_last == std::string::npos)
    return "";
  return s.substr(pos_first, pos_last - pos_first + 1);
}

std::string StrToLower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  return s;
}

std::string StrToUpper(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), ::toupper);
  return s;
}

bool IsStrUpper(const std::string &s) {
  for (size_t i = 0; i < s.size(); ++i) {
    if (s[i] < 'A' || 'Z' < s[i]) return false;
  }
  return true;
}

std::string CapitalizeHyphenSeparatedWords(std::string s) {
  // e.g. "content-length" -> "Content-Length"
  for (size_t i = 0; i < s.size(); ++i) {
    if (i == 0 || s[i - 1] == '-') {
      s[i] = toupper(s[i]);
    } else {
      s[i] = tolower(s[i]);
    }
  }
  return s;
}
}  // namespace string_utils
