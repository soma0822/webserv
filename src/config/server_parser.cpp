#include "server_parser.hpp"

bool ServerParser::parsed_root_;
bool ServerParser::parsed_ip_;
bool ServerParser::parsed_port_;
bool ServerParser::parsed_server_name_;
bool ServerParser::parsed_index_;
std::map<std::string, std::map<std::string, std::set<std::string> > >
    ServerParser::parsed_pair_;

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
      if (IsValidLocationKey(value) == false)
        throw std::invalid_argument("無効なlocation: " + line);
      LocationContext loc = LocationParser::ParseLocation(inf);
      std::string loc_key = MakeLocationKey(value);
      loc.SetPath(loc_key);
      server.AddLocation(loc_key, loc);
    } else {  // それ以外
      std::map<std::string, parseFunction>::iterator it = func.find(key);
      if (it == func.end()) {  // 対応した関数が見つからない
        throw std::invalid_argument("無効なserverキー: " + key);
      }
      if ((*it->second)(value, server) == false) {  // 関数が失敗した場合
        throw std::invalid_argument("無効なserver値: " +
                                    container::MergeContainer(value, " "));
      }
    }
  }
  if (server.IsValidContext() == false || UniqueServerName(server) == false)
    throw std::invalid_argument("serverにポートがありません");
  return server;
}

bool ServerParser::IsValidLocationKey(const std::vector<std::string> &value) {
  if (value.size() == 2) {
    if (validation::IsPath(value.at(0)) && value.at(1) == "{") return true;
  } else if (value.size() == 3) {
    if (value.at(0) == "=" && validation::IsPath(value.at(1)) &&
        value.at(2) == "{")
      return true;
  }
  return false;
}

std::string ServerParser::MakeLocationKey(
    const std::vector<std::string> &value) {
  if (value.size() == 2)
    return value.at(0);
  else
    return (value.at(0) + " " + value.at(1));
}

void ServerParser::ParseFuncInit(std::map<std::string, parseFunction> &func) {
  func["host"] = &ServerParser::ParseIp;
  func["index"] = &ServerParser::ParseIndex;
  func["root"] = &ServerParser::ParseRoot;
  func["server_name"] = &ServerParser::ParseServer_name;
  func["listen"] = &ServerParser::ParsePort;
  parsed_root_ = false;
  parsed_ip_ = false;
  parsed_port_ = false;
  parsed_server_name_ = false;
  parsed_index_ = false;
}

bool ServerParser::UniqueServerName(const ServerContext &server) {
  const std::string &port = server.GetPort();
  const std::string &ip = server.GetIp();
  const std::string &server_name = server.GetServerName();
  if (parsed_pair_[port][ip].insert(server_name).second == false) return false;
  return true;
}

bool ServerParser::UniqueListen() {
  std::map<std::string,
           std::map<std::string, std::set<std::string> > >::iterator it =
      parsed_pair_.begin();
  for (; it != parsed_pair_.end(); ++it) {
    if (it->second.size() > 1 && it->second.count("") > 0) return false;
  }
  return true;
}

// パーサー
bool ServerParser::ParseIndex(const std::vector<std::string> &value,
                              ServerContext &server) {
  if (parsed_index_ == true) throw std::invalid_argument("indexが複数あります");
  if (value.size() != 1) return false;
  server.SetIndex(value.at(0));
  parsed_index_ = true;
  return true;
}
bool ServerParser::ParseIp(const std::vector<std::string> &value,
                           ServerContext &server) {
  if (parsed_ip_ == true) throw std::invalid_argument("hostが複数あります");
  if (value.size() != 1) return false;
  server.SetIp(value.at(0));
  parsed_ip_ = true;
  return true;
}
bool ServerParser::ParseRoot(const std::vector<std::string> &value,
                             ServerContext &server) {
  if (parsed_root_ == true) throw std::invalid_argument("rootが複数あります");
  if (value.size() != 1) return false;
  server.SetRoot(value.at(0));
  parsed_root_ = true;
  return true;
}
bool ServerParser::ParseServer_name(const std::vector<std::string> &value,
                                    ServerContext &server) {
  if (parsed_server_name_ == true)
    throw std::invalid_argument("server_nameが複数あります");
  if (value.size() != 1) return false;
  server.SetServerName(value.at(0));
  parsed_server_name_ = true;
  return true;
}
bool ServerParser::ParsePort(const std::vector<std::string> &value,
                             ServerContext &server) {
  if (parsed_port_ == true) throw std::invalid_argument("portが複数あります");
  if (value.size() != 1) return false;
  if (validation::IsPort(value[0]) == false) return false;
  server.SetPort(value.at(0));
  parsed_port_ = true;
  return true;
}

void ServerParser::RemoveSemicolon(std::string &line) {
  std::stringstream ss(line);
  std::string key;
  ss >> key;
  if (key == "location" || key == "}") {
    return;
  }
  if (line.at(line.size() - 1) == ';') {
    line.erase(line.size() - 1, 1);
    return;
  }
  throw std::invalid_argument("Syntaxエラー: semicolon: " + line);
}

void ServerParser::ClearParsedPair() { parsed_pair_.clear(); }
