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
    if ((*it->second)(value, location) == false) {  // 関数が失敗した場合
      throw std::invalid_argument("Invalid location value: " +
                                  container::MergeContainer(value, " "));
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
  location.SetCanAutoIndex(value.at(0) == "on" ? true : false);
  return true;
}
bool LocationParser::ParseLimitClientBody(const std::vector<std::string> &value,
                                          LocationContext &location) {
  if (value.size() != 1) return false;
  Result<int, std::string> result = string_utils::StrToI(value.at(0));
  if (result.IsErr()) return false;
  location.SetLimitClientBody(result.Unwrap());
  return true;
}
bool LocationParser::ParseReturn(const std::vector<std::string> &value,
                                 LocationContext &location) {
  if (value.size() != 1) return false;
  location.SetReturn(value.at(0));
  return true;
}
bool LocationParser::ParseAlias(const std::vector<std::string> &value,
                                LocationContext &location) {
  if (value.size() != 1) return false;
  location.SetAlias(value.at(0));
  return true;
}
bool LocationParser::ParseRoot(const std::vector<std::string> &value,
                               LocationContext &location) {
  if (value.size() != 1) return false;
  location.SetRoot(value.at(0));
  return true;
}
bool LocationParser::ParseIndex(const std::vector<std::string> &value,
                                LocationContext &location) {
  if (value.size() == 0) return false;
  for (std::vector<std::string>::const_iterator it = value.begin();
       it != value.end(); it++) {
    location.AddIndex(*it);
  }
  return true;
}
bool LocationParser::ParseCgiPath(const std::vector<std::string> &value,
                                  LocationContext &location) {
  if (value.size() == 0) return false;
  for (std::vector<std::string>::const_iterator it = value.begin();
       it != value.end(); it++) {
    if (validation::IsPath(*it) == false) return false;
    location.AddCgiPath(*it);
  }
  return true;
}
bool LocationParser::ParseCgiExtention(const std::vector<std::string> &value,
                                       LocationContext &location) {
  if (value.size() == 0) return false;
  for (std::vector<std::string>::const_iterator it = value.begin();
       it != value.end(); it++) {
    location.AddCgiExtention(*it);
  }
  return true;
}
bool LocationParser::ParseAllowMethod(const std::vector<std::string> &value,
                                      LocationContext &location) {
  if (value.size() == 0) return false;
  for (std::vector<std::string>::const_iterator it = value.begin();
       it != value.end(); it++) {
    location.AddAllowMethod(*it);
  }
  return true;
}
bool LocationParser::ParseErrorPage(const std::vector<std::string> &value,
                                    LocationContext &location) {
  if (value.size() < 2) return false;
  for (std::vector<std::string>::const_iterator it = value.begin();
       it != value.end() - 1; it++) {
    if (validation::IsNumber(*it) == false) return false;
    location.AddErrorPage(*it, *(value.end() - 1));
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
