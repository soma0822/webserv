#include "location_parser.hpp"

LocationContext LocationParser::parse_location(std::ifstream &inf){
  LocationContext location;
  std::map<std::string, parseFunction> func;
  std::string line;

  init_parse_func(func);
  while (std::getline(inf, line)){
    if (line.empty())
      continue;
    remove_semicolon(line);
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
      std::cerr << "Invalid location key: " << key << std::endl;
      throw std::exception();
    }
    if ((*it->second)(value, location) == false){  //対応した関数に適切な要素数と異なっている
      std::cerr << "Invalid location value: ";
      for (std::vector<std::string>::const_iterator it = value.begin(); it != value.end(); it++)
        std::cerr << *it;
      std::cerr << std::endl;
      throw std::exception();
    }
  }
  return location; 
}

void LocationParser::init_parse_func(std::map<std::string, parseFunction> &func){
  func["auto_index"] = &LocationParser::parse_auto_index;
  func["limit_client_body"] = &LocationParser::parse_limit_client_body;
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
    return false;
  location.set_return(value[0]);
  return true;
}
bool LocationParser::parse_alias(const std::vector<std::string> &value, LocationContext &location){
  if (value.size() != 1)
    return false;
  location.set_alias(value[0]);
  return true;
}
bool LocationParser::parse_root(const std::vector<std::string> &value, LocationContext &location){
  if (value.size() != 1)
    return false;
  location.set_root(value[0]);
  return true;
}
bool LocationParser::parse_index(const std::vector<std::string> &value, LocationContext &location){
  if (value.size() == 0)
    return false;
  for (std::vector<std::string>::const_iterator it = value.begin(); it != value.end(); it++){
    location.add_index(*it);
  }
  return true;
}
bool LocationParser::parse_cgi_path(const std::vector<std::string> &value, LocationContext &location){
  if (value.size() == 0)
    return false;
  for (std::vector<std::string>::const_iterator it = value.begin(); it != value.end(); it++){
    location.add_cgi_path(*it);
  }
  return true;
}
bool LocationParser::parse_cgi_extention(const std::vector<std::string> &value, LocationContext &location){
  if (value.size() == 0)
    return false;
  for (std::vector<std::string>::const_iterator it = value.begin(); it != value.end(); it++){
    location.add_cgi_extention(*it);
  }
  return true;
}
bool LocationParser::parse_allow_method(const std::vector<std::string> &value, LocationContext &location){
  if (value.size() == 0)
    return false;
  for (std::vector<std::string>::const_iterator it = value.begin(); it != value.end(); it++){
    location.add_allow_method(*it);
  }
  return true;
}
bool LocationParser::parse_error_page(const std::vector<std::string> &value, LocationContext &location){
  if (value.size() != 2)
    return false;
  location.add_errorPage(value[0], value[1]);
  return true;
}

void LocationParser::remove_semicolon(std::string &line){
	std::stringstream ss(line);
	std::string key;
	ss >> key;
	if (key == "}"){
		return ;
	}
	if (line.back() == ';'){
		line.pop_back();
		return ;
	}
	std::cerr << "Syntax error: semicolon." << std::endl;
  throw std::exception();
}