#ifndef WEBSERV_UTILS_STRING_UTILS_HPP
#define WEBSERV_UTILS_STRING_UTILS_HPP

#include <climits>
#include <string>

#include "result.hpp"
#include "validation.hpp"

namespace string_utils {
Result<int, std::string> StrToI(const std::string &str);
Result<int, std::string> StrToHex(const std::string &str);
std::string SkipSpace(std::string s);
}  // namespace string_utils
#endif
