#ifndef SERVER_PARSER_HPP
# define SERVER_PARSER_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include "server_context.hpp"

class ServerParser{
public:
  static ServerContext parse_server(std::ifstream &);
};

#endif