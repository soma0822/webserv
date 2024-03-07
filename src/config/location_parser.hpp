#ifndef WEBSERV_SRC_CONFIG_LOCATION_PARSER_HPP
#define WEBSERV_SRC_CONFIG_LOCATION_PARSER_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

#include "container.hpp"
#include "location_context.hpp"
#include "result.hpp"
#include "string_utils.hpp"
#include "validation.hpp"

class LocationParser {
 public:
  typedef bool (*parseFunction)(const std::vector<std::string> &value,
                                LocationContext &location);
  static LocationContext ParseLocation(std::ifstream &inf);

 private:
  static void ParseFuncInit(std::map<std::string, parseFunction> &func);
  static bool ParseAutoIndex(const std::vector<std::string> &value,
                             LocationContext &location);
  static bool ParseLimitClientBody(const std::vector<std::string> &value,
                                   LocationContext &location);
  static bool ParseReturn(const std::vector<std::string> &value,
                          LocationContext &location);
  static bool ParseAlias(const std::vector<std::string> &value,
                         LocationContext &location);
  static bool ParseRoot(const std::vector<std::string> &value,
                        LocationContext &location);
  static bool ParseIndex(const std::vector<std::string> &value,
                         LocationContext &location);
  static bool ParseCgiPath(const std::vector<std::string> &value,
                           LocationContext &location);
  static bool ParseCgiExtention(const std::vector<std::string> &value,
                                LocationContext &location);
  static bool ParseAllowMethod(const std::vector<std::string> &value,
                               LocationContext &location);
  static bool ParseErrorPage(const std::vector<std::string> &value,
                             LocationContext &location);
  static void RemoveSemicolon(std::string &line);
  static bool parsed_auto_index_;
  static bool parsed_limit_client_body_;
  static bool parsed_return_;
  static bool parsed_alias_;
  static bool parsed_root_;
  static bool parsed_index_;
};

#endif
