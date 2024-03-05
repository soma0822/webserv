#ifndef WEBSERV_SRC_CONFIG_SERVER_PARSER_HPP
#define WEBSERV_SRC_CONFIG_SERVER_PARSER_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

#include "container.hpp"
#include "location_context.hpp"
#include "location_parser.hpp"
#include "server_context.hpp"
#include "validation.hpp"

class ServerParser {
 public:
  typedef bool (*parseFunction)(const std::vector<std::string> &value,
                                ServerContext &server);
  static ServerContext ParseServer(std::ifstream &inf);

 private:
  static bool IsValidLocationKey(const std::vector<std::string> &value);
  static std::string MakeLocationKey(const std::vector<std::string> &value);
  static void ParseFuncInit(std::map<std::string, parseFunction> &func);
  static bool ParseErrorPage(const std::vector<std::string> &value,
                             ServerContext &server);
  static bool ParseIndex(const std::vector<std::string> &value,
                         ServerContext &server);
  static bool ParseIp(const std::vector<std::string> &value,
                      ServerContext &server);
  static bool ParseRoot(const std::vector<std::string> &value,
                        ServerContext &server);
  static bool ParseServer_name(const std::vector<std::string> &value,
                               ServerContext &server);
  static bool ParsePort(const std::vector<std::string> &value,
                        ServerContext &server);
  static void RemoveSemicolon(std::string &line);
  static bool parsed_root_;
  static bool parsed_ip_;
};

#endif
