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

class ConfigParser {
 public:
  static Config Parse(const std::string &);
};

#endif
