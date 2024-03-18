#include "location_parser.hpp"

bool LocationParser::parsed_auto_index_;
bool LocationParser::parsed_limit_client_body_;
bool LocationParser::parsed_return_;
bool LocationParser::parsed_alias_;
bool LocationParser::parsed_root_;
bool LocationParser::parsed_index_;

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
      throw std::invalid_argument("無効なlocationキー: " + key);
    }
    if ((*it->second)(value, location) == false) {  // 関数が失敗した場合
      throw std::invalid_argument("無効なlocation値: " +
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
  func["cgi_extension"] = &LocationParser::ParseCgiExtension;
  func["allow_methods"] = &LocationParser::ParseAllowMethod;
  parsed_auto_index_ = false;
  parsed_limit_client_body_ = false;
  parsed_return_ = false;
  parsed_alias_ = false;
  parsed_root_ = false;
  parsed_index_ = false;
}

bool LocationParser::ParseAutoIndex(const std::vector<std::string> &value,
                                    LocationContext &location) {
  if (parsed_auto_index_ == true)
    throw std::invalid_argument("auto_indexが複数あります");
  if (value.size() != 1) return false;
  location.SetCanAutoIndex(value.at(0) == "on");
  parsed_auto_index_ = true;
  return true;
}
bool LocationParser::ParseLimitClientBody(const std::vector<std::string> &value,
                                          LocationContext &location) {
  if (parsed_limit_client_body_ == true)
    throw std::invalid_argument("limit_client_bodyが複数あります");
  if (value.size() != 1) return false;
  Result<int, std::string> result = string_utils::StrToI(value.at(0));
  if (result.IsErr()) return false;
  location.SetLimitClientBody(result.Unwrap());
  parsed_limit_client_body_ = true;
  return true;
}
bool LocationParser::ParseReturn(const std::vector<std::string> &value,
                                 LocationContext &location) {
  if (parsed_return_ == true)
    throw std::invalid_argument("returnが複数あります");
  if (value.size() != 1) return false;
  location.SetReturn(value.at(0));
  parsed_return_ = true;
  return true;
}
bool LocationParser::ParseAlias(const std::vector<std::string> &value,
                                LocationContext &location) {
  if (parsed_alias_ == true) throw std::invalid_argument("aliasが複数あります");
  if (value.size() != 1) return false;
  location.SetAlias(value.at(0));
  parsed_alias_ = true;
  return true;
}
bool LocationParser::ParseRoot(const std::vector<std::string> &value,
                               LocationContext &location) {
  if (parsed_root_ == true) throw std::invalid_argument("rootが複数あります");
  if (value.size() != 1) return false;
  location.SetRoot(value.at(0));
  parsed_root_ = true;
  return true;
}
bool LocationParser::ParseIndex(const std::vector<std::string> &value,
                                LocationContext &location) {
  if (parsed_index_ == true) throw std::invalid_argument("indexが複数あります");
  if (value.size() != 1) return false;
  location.SetIndex(value.at(0));
  parsed_index_ = true;
  return true;
}
bool LocationParser::ParseCgiPath(const std::vector<std::string> &value,
                                  LocationContext &location) {
  if (value.size() == 0) return false;
  for (unsigned int i = 0; i < value.size(); ++i) {
    if (validation::IsPath(value.at(i)) == false) return false;
    location.AddCgiPath(value.at(i));
  }
  return true;
}
bool LocationParser::ParseCgiExtension(const std::vector<std::string> &value,
                                       LocationContext &location) {
  if (value.size() == 0) return false;
  for (unsigned int i = 0; i < value.size(); ++i)
    location.AddCgiExtension(value.at(i));
  return true;
}
bool LocationParser::ParseAllowMethod(const std::vector<std::string> &value,
                                      LocationContext &location) {
  if (value.size() == 0) return false;
  for (unsigned int i = 0; i < value.size(); ++i)
    location.AddAllowMethod(value.at(i));
  return true;
}

void LocationParser::RemoveSemicolon(std::string &line) {
  std::stringstream ss(line);
  std::string key;
  ss >> key;
  if (key == "}") {
    return;
  }
  if (line.at(line.size() - 1) == ';') {
    line.erase(line.size() - 1, 1);
    return;
  }
  throw std::invalid_argument("Syntaxエラー: semicolon: " + line);
}
