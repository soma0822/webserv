#ifndef WEBSERV_UTILS_STR_TO_I_HPP
#define WEBSERV_UTILS_STR_TO_I_HPP

#include <string>

#include "result.hpp"
#include "validation.hpp"

Result<int, std::string> StrToI(const std::string &str);

#endif
