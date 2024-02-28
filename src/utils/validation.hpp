#ifndef WEBSERV_SRC_UTILS_VALIDATION_HPP
#define WEBSERV_SRC_UTILS_VALIDATION_HPP

#include <sstream>
#include <string>
#include <vector>

#include "result.hpp"
#include "str_to_i.hpp"
namespace validation {
bool IsNumber(const std::string &);
bool IsPath(const std::string &);
bool IsIp(const std::string &);
bool IsPort(const std::string &);
static const int kMaxPort = 65535;
}  // namespace validation

#endif
