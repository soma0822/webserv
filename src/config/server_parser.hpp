#ifndef WEBSERV_SRC_CONFIG_SERVER_PARSER_HPP
#define WEBSERV_SRC_CONFIG_SERVER_PARSER_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

#include "location_context.hpp"
#include "location_parser.hpp"
#include "merge_string_and_container.hpp"
#include "server_context.hpp"
#include "validation.hpp"

class ServerParser {
 public:
  typedef bool (*parseFunction)(const std::vector<std::string> &,
                                ServerContext &);
  static ServerContext ParseServer(std::ifstream &);

 private:
  static void ParseFuncInit(std::map<std::string, parseFunction> &);
  static bool ParseErrorPage(const std::vector<std::string> &, ServerContext &);
  static bool ParseIndex(const std::vector<std::string> &, ServerContext &);
  static bool ParseIp(const std::vector<std::string> &, ServerContext &);
  static bool ParseRoot(const std::vector<std::string> &, ServerContext &);
  static bool ParseServer_name(const std::vector<std::string> &,
                               ServerContext &);
  static bool ParsePort(const std::vector<std::string> &, ServerContext &);
  static void RemoveSemicolon(std::string &);
};

#endif
