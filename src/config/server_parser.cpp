#include "server_parser.hpp"

ServerContext ServerParser::ParseServer(std::ifstream &inf) {
  ServerContext server;
  std::map<std::string, parseFunction> func;
  std::string line;

  ParseFuncInit(func);
  while (std::getline(inf, line)) {
    if (line.empty()) continue;
    RemoveSemicolon(line);
    std::stringstream ss(line);
    std::string key, tmp;
    std::vector<std::string> value;
    ss >> key;
    while (ss >> tmp) value.push_back(tmp);
    if (key == "}" && value.size() == 0) break;
    if (key == "location") {  // locationの時
      if (value.size() != 2 || value.at(1) != "{") {
        throw std::invalid_argument("Syntax error: " + line);
      }
      if (validation::IsPath(value.at(0)) == false) {
        throw std::invalid_argument("Invalid location path: " + value.at(0));
      }
      server.AddLocation(value.at(0), LocationParser::ParseLocation(inf));
    } else {  // それ以外
      std::map<std::string, parseFunction>::iterator it = func.find(key);
      if (it == func.end()) {  // 対応した関数が見つからない
        throw std::invalid_argument("Invalid server key: " + key);
      }
      if ((*it->second)(value, server) == false) {  // 関数が失敗した場合
        throw std::invalid_argument(
            MergeStringAndContainer("Invalid server value: ", value));
      }
    }
  }
  return server;
}

void ServerParser::ParseFuncInit(std::map<std::string, parseFunction> &func) {
  func["host"] = &ServerParser::ParseIp;
  func["error_page"] = &ServerParser::ParseErrorPage;
  func["index"] = &ServerParser::ParseIndex;
  func["root"] = &ServerParser::ParseRoot;
  func["server_name"] = &ServerParser::ParseServer_name;
  func["listen"] = &ServerParser::ParsePort;
}

// パーサー
bool ServerParser::ParseErrorPage(const std::vector<std::string> &value,
                                  ServerContext &server) {
  if (value.size() < 2) return false;
  for (std::vector<std::string>::const_iterator it = value.begin();
       it != value.end() - 1; it++) {
    if (validation::IsNumber(*it) == false) return false;
    server.AddErrorPage(*it, *(value.end() - 1));
  }
  return true;
}
bool ServerParser::ParseIndex(const std::vector<std::string> &value,
                              ServerContext &server) {
  if (value.size() == 0) return false;
  server.AddIndex(value.at(0));
  return true;
}
bool ServerParser::ParseIp(const std::vector<std::string> &value,
                           ServerContext &server) {
  if (value.size() != 1) return false;
  server.SetIp(value.at(0));
  return true;
}
bool ServerParser::ParseRoot(const std::vector<std::string> &value,
                             ServerContext &server) {
  if (value.size() != 1) return false;
  server.SetRoot(value.at(0));
  return true;
}
bool ServerParser::ParseServer_name(const std::vector<std::string> &value,
                                    ServerContext &server) {
  if (value.size() == 0) return false;
  for (std::vector<std::string>::const_iterator it = value.begin();
       it != value.end(); it++)
    server.AddServerName(*it);
  return true;
}
bool ServerParser::ParsePort(const std::vector<std::string> &value,
                             ServerContext &server) {
  if (value.size() != 1) return false;
  if (validation::IsPort(value.at(0)) == false) return false;
  server.AddPort(value.at(0));
  return true;
}

void ServerParser::RemoveSemicolon(std::string &line) {
  std::stringstream ss(line);
  std::string key;
  ss >> key;
  if (key == "location" || key == "}") {
    return;
  }
  if (line.back() == ';') {
    line.pop_back();
    return;
  }
  throw std::invalid_argument("Syntax error: semicolon: " + line);
}
