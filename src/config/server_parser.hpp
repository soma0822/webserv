#ifndef SERVER_PARSER_HPP
# define SERVER_PARSER_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include "server_context.hpp"
#include "location_context.hpp"
#include "location_parser.hpp"

class ServerParser{
public:
  typedef bool  (*parseFunction)(const std::vector<std::string> &, ServerContext &);
  static ServerContext parse_server(std::ifstream &);

private:
  static void init_parse_func(std::map<std::string, parseFunction> &);
  static bool parse_error_page(const std::vector<std::string> &, ServerContext &);
  static bool parse_index(const std::vector<std::string> &, ServerContext &);
  static bool parse_ip(const std::vector<std::string> &, ServerContext &);
  static bool parse_root(const std::vector<std::string> &, ServerContext &);
  static void remove_semicolon(std::string &);
};

#endif