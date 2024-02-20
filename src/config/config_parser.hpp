#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <iostream>
#include <vector>
#include "server_context.hpp"

class ConfigParser{
public:
  static std::vector<ServerContext> parse(const std::string &);

};

#endif