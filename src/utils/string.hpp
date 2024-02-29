#ifndef WEBSERV_UTILS_STRING_HPP
#define WEBSERV_UTILS_STRING_HPP

#include <climits>
#include <string>

#include "result.hpp"
#include "validation.hpp"

namespace string {
Result<int, std::string> StrToI(const std::string &str);
}  // namespace string
#endif