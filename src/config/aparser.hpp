#ifndef WEBSERV_SRC_CONFIG_APARSER_HPP
#define WEBSERV_SRC_CONFIG_APARSER_HPP

#include <climits>
#include <stdexcept>
#include <string>

class AParser {
 protected:
  static bool IsNum(const std::string &str);
  static bool IsPath(const std::string &str);
  static int StrToI(std::string const &str);
};

#endif
