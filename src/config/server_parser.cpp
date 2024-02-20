#include "server_parser.hpp"

ServerContext ServerParser::parse_server(std::ifstream &inf){
  ServerContext server;
  std::map<std::string, LocationContext> location;
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
    
    if (key == "location"){  // locationの時
      if (value.size() != 3 || value[2] != "{"){
        std::cerr << "Syntax error: " << line << std::endl;
        throw std::exception();
      }
      LocationContext loc = LocationParser::parse_location();
      if (location.find(value[1]) == location.end())
        location[value[1]] = loc;
    }
    //それ以外
    std::map<std::string, parseFunction>::iterator it = func.find(key);
    if (it == func.end()){  //対応した関数が見つからない
      std::cerr << "Valid server content: " << key << std::endl;
      throw std::exception();
    }
    (*it->second)(value, server);
  }
  return server;
}

void ServerParser::init_parse_func(std::map<std::string, parseFunction> &func){
	func["ip"] = &ServerParser::parse_ip;
	func["error_page"] = &ServerParser::parse_error_page;
	func["index"] = &ServerParser::parse_index;
	func["root"] = &ServerParser::parse_root;
}

// パーサー
bool ServerParser::parse_error_page(const std::vector<std::string> &value, ServerContext &server){
  if (value.size() != 2)
    return false;
  server.add_error_page(value[0], value[1]);
  return true;
}
bool ServerParser::parse_index(const std::vector<std::string> &value, ServerContext &server){
  if (value.size() == 0)
    return false;
  server.add_index(value[0]);
  return true;
}
bool ServerParser::parse_ip(const std::vector<std::string> &value, ServerContext &server){
  if (value.size() != 1)
    return false;
  server.set_ip(value[0]);
  return true;
}
bool ServerParser::parse_root(const std::vector<std::string> &value, ServerContext &server){
  if (value.size() != 1)
    return false;
  server.set_root(value[0]);
  return true;
}

void ServerParser::remove_semicolon(std::string &line){
	std::stringstream ss(line);
	std::string key;
	ss >> key;
	if (key == "location" || key == "}"){
		return ;
	}
	if (line.back() == ';'){
		line.pop_back();
		return ;
	}
	std::cerr << "Syntax error: semicolon." << std::endl;
  throw std::exception();
}