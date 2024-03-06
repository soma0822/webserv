#include "server_parser.hpp"

bool ServerParser::parsed_root_;
bool ServerParser::parsed_ip_;
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
      server.AddLocation(MakeLocationKey(value),
                         LocationParser::ParseLocation(inf));
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
  if (server.IsValidContext() && UniqueListen(server))
    return server;
  else
    throw std::invalid_argument("不正なserver設定です");
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

bool ServerParser::UniqueListen(const ServerContext &server) {
  const std::string &ip = server.GetIp();
  const std::vector<std::string> &ports = server.GetPort();
  const std::vector<std::string> &server_names = server.GetServerName();
  for (std::vector<std::string>::const_iterator port_it = ports.begin();
       port_it != ports.end(); ++port_it) {
    for (std::vector<std::string>::const_iterator server_name_it =
             server_names.begin();
         server_name_it != server_names.end(); ++server_name_it) {
      std::map<std::string,
               std::map<std::string, std::set<std::string> > >::iterator
          find_port = parsed_pair_.find(*port_it);
      if (parsed_pair_.end() != find_port) {
        if (ip == "") {
          if (find_port->second.size() == 0 ||
              (find_port->second.size() == 1 &&
               find_port->second.end() != find_port->second.find(""))) {
            ;
          } else
            return false;
        } else {
          if (find_port->second.end() == find_port->second.find("")) {
            ;
          } else
            return false;
        }
      }
      if (parsed_pair_[*port_it][ip].insert(*server_name_it).second)
        ;
      else
        return false;
    }
  }
  return true;
}

void ServerParser::ParseFuncInit(std::map<std::string, parseFunction> &func) {
  func["host"] = &ServerParser::ParseIp;
  func["error_page"] = &ServerParser::ParseErrorPage;
  func["index"] = &ServerParser::ParseIndex;
  func["root"] = &ServerParser::ParseRoot;
  func["server_name"] = &ServerParser::ParseServer_name;
  func["listen"] = &ServerParser::ParsePort;
  parsed_root_ = false;
  parsed_ip_ = false;
}

// パーサー
bool ServerParser::ParseErrorPage(const std::vector<std::string> &value,
                                  ServerContext &server) {
  if (value.size() < 2) return false;
  for (unsigned int i = 0; i < value.size() - 1; ++i) {
    if (validation::IsNumber(value.at(i)) == false) return false;
    server.AddErrorPage(value.at(i), *(value.end() - 1));
  }
  return true;
}
bool ServerParser::ParseIndex(const std::vector<std::string> &value,
                              ServerContext &server) {
  if (value.size() == 0) return false;
  for (unsigned int i = 0; i < value.size(); ++i) server.AddIndex(value.at(i));
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
  if (value.size() == 0) return false;
  for (unsigned int i = 0; i < value.size(); ++i) {
    server.AddServerName(value.at(i));
  }
  return true;
}
bool ServerParser::ParsePort(const std::vector<std::string> &value,
                             ServerContext &server) {
  if (value.size() == 0) return false;
  for (unsigned int i = 0; i < value.size(); ++i) {
    if (validation::IsPort(value.at(i)) == false) return false;
    server.AddPort(value.at(i));
  }
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
