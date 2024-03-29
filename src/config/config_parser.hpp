#ifndef WEBSERV_SRC_CONFIG_CONFIG_PARSER_HPP
#define WEBSERV_SRC_CONFIG_CONFIG_PARSER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "config.hpp"
#include "server_context.hpp"
#include "server_parser.hpp"

class Config;

class ConfigParser {
 public:
  static Config Parse(const std::string &file);
  static bool ParseErrorPage(const std::string &line, Config &config);
  static void RemoveSemicolon(std::string &line);
  static const std::string default_file_;
};

#endif
