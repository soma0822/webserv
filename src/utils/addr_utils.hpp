#ifndef WEBSERV_SRC_UTILS_ADDR_UTILS_HPP
#define WEBSERV_SRC_UTILS_ADDR_UTILS_HPP

#include <sstream>
#include "result.hpp"
#include "string_utils.hpp"

namespace addr_utils{
  uint32_t InetAddr(const std::string& ip_str);
}




#endif
