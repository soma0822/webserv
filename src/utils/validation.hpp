#ifndef WEBSERV_SRC_UTILS_VALIDATION_HPP
#define WEBSERV_SRC_UTILS_VALIDATION_HPP

#include <sstream>
#include <string>
#include <vector>

#include "result.hpp"
#include "string_utils.hpp"
namespace validation {
bool IsNumber(const std::string &str);
bool IsPath(const std::string &str);
bool IsIp(const std::string &str);
bool IsPort(const std::string &str);
static const int kMaxPort = 65535;
}  // namespace validation

#endif
