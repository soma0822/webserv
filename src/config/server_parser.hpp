#ifndef SERVER_PARSER_HPP
#define SERVER_PARSER_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

#include "aparser.hpp"
#include "location_context.hpp"
#include "location_parser.hpp"
#include "server_context.hpp"

class ServerParser : public AParser {
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
