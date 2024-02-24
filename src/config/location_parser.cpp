#include "location_parser.hpp"

LocationContext LocationParser::ParseLocation(std::ifstream &inf) {
  LocationContext location;
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
    std::map<std::string, parseFunction>::iterator it = func.find(key);
    if (it == func.end()) {  // 対応した関数が見つからない
      throw std::invalid_argument("Invalid location key: " + key);
    }
    if ((*it->second)(value, location) ==
        false) {  // 対応した関数に適切な要素数と異なっている
      std::string error = "Invalid location value:";
      for (std::vector<std::string>::const_iterator it = value.begin();
           it != value.end(); it++) {
        error += " " + *it;
      }
      throw std::invalid_argument(error);
    }
  }
  return location;
}

void LocationParser::ParseFuncInit(std::map<std::string, parseFunction> &func) {
  func["autoindex"] = &LocationParser::ParseAutoIndex;
  func["limit_client_body"] = &LocationParser::ParseLimitClientBody;
  func["return"] = &LocationParser::ParseReturn;
  func["alias"] = &LocationParser::ParseAlias;
  func["root"] = &LocationParser::ParseRoot;
  func["index"] = &LocationParser::ParseIndex;
  func["cgi_path"] = &LocationParser::ParseCgiPath;
  func["cgi_ext"] = &LocationParser::ParseCgiExtention;
  func["allow_methods"] = &LocationParser::ParseAllowMethod;
  func["error_page"] = &LocationParser::ParseErrorPage;
}

bool LocationParser::ParseAutoIndex(const std::vector<std::string> &value,
                                    LocationContext &location) {
  if (value.size() != 1) return false;
  location.set_can_auto_index(value[0] == "on" ? true : false);
  return true;
}
bool LocationParser::ParseLimitClientBody(const std::vector<std::string> &value,
                                          LocationContext &location) {
  if (value.size() != 1) return false;
  location.set_limit_client_body(StrToI(value[0]));
  return true;
}
bool LocationParser::ParseReturn(const std::vector<std::string> &value,
                                 LocationContext &location) {
  if (value.size() != 1) return false;
  location.set_return(value[0]);
  return true;
}
bool LocationParser::ParseAlias(const std::vector<std::string> &value,
                                LocationContext &location) {
  if (value.size() != 1) return false;
  location.set_alias(value[0]);
  return true;
}
bool LocationParser::ParseRoot(const std::vector<std::string> &value,
                               LocationContext &location) {
  if (value.size() != 1) return false;
  location.set_root(value[0]);
  return true;
}
bool LocationParser::ParseIndex(const std::vector<std::string> &value,
                                LocationContext &location) {
  if (value.size() == 0) return false;
  for (std::vector<std::string>::const_iterator it = value.begin();
       it != value.end(); it++) {
    location.add_index(*it);
  }
  return true;
}
bool LocationParser::ParseCgiPath(const std::vector<std::string> &value,
                                  LocationContext &location) {
  if (value.size() == 0) return false;
  for (std::vector<std::string>::const_iterator it = value.begin();
       it != value.end(); it++) {
    if (IsPath(*it) == false) return false;
    location.add_cgi_path(*it);
  }
  return true;
}
bool LocationParser::ParseCgiExtention(const std::vector<std::string> &value,
                                       LocationContext &location) {
  if (value.size() == 0) return false;
  for (std::vector<std::string>::const_iterator it = value.begin();
       it != value.end(); it++) {
    location.add_cgi_extention(*it);
  }
  return true;
}
bool LocationParser::ParseAllowMethod(const std::vector<std::string> &value,
                                      LocationContext &location) {
  if (value.size() == 0) return false;
  for (std::vector<std::string>::const_iterator it = value.begin();
       it != value.end(); it++) {
    location.add_allow_method(*it);
  }
  return true;
}
bool LocationParser::ParseErrorPage(const std::vector<std::string> &value,
                                    LocationContext &location) {
  if (value.size() < 2) return false;
  for (std::vector<std::string>::const_iterator it = value.begin();
       it != value.end() - 1; it++) {
    if (IsNum(*it) == false) return false;
    location.add_error_page(*it, *(value.end() - 1));
  }
  return true;
}

void LocationParser::RemoveSemicolon(std::string &line) {
  std::stringstream ss(line);
  std::string key;
  ss >> key;
  if (key == "}") {
    return;
  }
  if (line.back() == ';') {
    line.pop_back();
    return;
  }
  throw std::invalid_argument("Syntax error: semicolon: " + line);
}
