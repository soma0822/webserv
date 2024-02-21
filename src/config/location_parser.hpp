#ifndef LOCATION_PARSER_HPP
#define LOCATION_PARSER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "location_context.hpp"

class LocationParser{
public:
  typedef bool  (*parseFunction)(const std::vector<std::string> &, LocationContext &);
  static LocationContext parse_location(std::ifstream &);

private:
  static void init_parse_func(std::map<std::string, parseFunction> &);
  static void remove_semicolon(std::string &line);
  static bool parse_auto_index(const std::vector<std::string> &, LocationContext &);
  static bool parse_limit_client_body(const std::vector<std::string> &, LocationContext &);
  static bool parse_return(const std::vector<std::string> &, LocationContext &);
  static bool parse_alias(const std::vector<std::string> &, LocationContext &);
  static bool parse_root(const std::vector<std::string> &, LocationContext &);
  static bool parse_index(const std::vector<std::string> &, LocationContext &);
  static bool parse_cgi_path(const std::vector<std::string> &, LocationContext &);
  static bool parse_cgi_extention(const std::vector<std::string> &, LocationContext &);
  static bool parse_allow_method(const std::vector<std::string> &, LocationContext &);
  static bool parse_error_page(const std::vector<std::string> &, LocationContext &);
};

#endif
