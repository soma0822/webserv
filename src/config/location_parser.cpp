#include "location_parser.hpp"

LocationContext LocationParser::parse_location(std::ifstream &inf){
  LocationContext location;
  std::map<std::string, parseFunction> func;
  std::string line;

  init_parse_func(func);
  while (std::getline(inf, line)){
    if (line.empty())
      continue;
    std::stringstream ss(line);
    std::string key, tmp;
    std::vector<std::string> value;
    ss >> key;
    while (ss >> tmp)
      value.push_back(tmp);
    if (key == "}" && value.size() == 0)
      break ;
    std::map<std::string, parseFunction>::iterator it = func.find(key);
    if (it == func.end()){  //対応した関数が見つからない
      std::cerr << "Valid location content: " << key << std::endl;
      throw std::exception();
    }
    if ((*it->second)(value, location) == false){  //対応した関数に適切な要素数と異なっている
      std::cerr << "Valid server content: ";
      for (std::vector<std::string>::const_iterator it = value.begin(); it != value.end(); it++)
        std::cerr << *it;
      std::cerr << std::endl;
      throw std::exception();
    }
  }
  return location; 
}

void LocationParser::init_parse_func(std::map<std::string, parseFunction> &func){
  func["return"] = &LocationParser::parse_return;
  func["alias"] = &LocationParser::parse_alias;
  func["root"] = &LocationParser::parse_root;
  func["index"] = &LocationParser::parse_index;
  func["cgi_path"] = &LocationParser::parse_cgi_path;
  func["cgi_extention"] = &LocationParser::parse_cgi_extention;
  func["allow_method"] = &LocationParser::parse_allow_method;
  func["error_page"] = &LocationParser::parse_error_page;
}


bool LocationParser::parse_auto_index(const std::vector<std::string> &value, LocationContext &location){
  if (value.size() != 1)
    return false;
  location.set_can_auto_index(value[0] == "on" ? true : false);
  return true;
}
bool LocationParser::parse_limit_client_body(const std::vector<std::string> &value, LocationContext &location){
  if (value.size() != 1)
    return false;
  location.set_limit_client_body(std::stoi(value[0]));
  return true;
}
bool LocationParser::parse_return(const std::vector<std::string> &value, LocationContext &location){
  if (value.size() != 1)
}
bool LocationParser::parse_alias(const std::vector<std::string> &value, LocationContext &location);
bool LocationParser::parse_root(const std::vector<std::string> &value, LocationContext &location);
bool LocationParser::parse_index(const std::vector<std::string> &value, LocationContext &location);
bool LocationParser::parse_cgi_path(const std::vector<std::string> &value, LocationContext &location);
bool LocationParser::parse_cgi_extention(const std::vector<std::string> &value, LocationContext &location);
bool LocationParser::parse_allow_method(const std::vector<std::string> &value, LocationContext &location);
bool LocationParser::parse_error_page(const std::vector<std::string> &value, LocationContext &location);