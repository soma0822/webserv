#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "server_context.hpp"
#include "server_parser.hpp"


class ConfigParser{
public:
  static std::vector<ServerContext> parse(std::ifstream &);
};

#endif