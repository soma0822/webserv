#ifndef WEBSERV_SRC_UTILS_VALIDATION_HPP
#define WEBSERV_SRC_UTILS_VALIDATION_HPP

#include <string>
#include <vector>
#include <sstream>

namespace validation {
bool IsNumber(const std::string &);
bool IsPath(const std::string &);
bool IsIp(const std::string &);
bool IsPort(const std::string &);
}

#endif

