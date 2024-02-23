#ifndef APARSER_HPP
#define APARSER_HPP

#include <string>
#include <climits>
#include <stdexcept>

class AParser{
protected:
  static bool is_num(const std::string &str);
  static bool is_path(const std::string &str);
  static int strtoi(std::string const &str);
};

#endif
