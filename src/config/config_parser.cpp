#include "config_parser.hpp"

std::vector<ServerContext> ConfigParser::parse(std::ifstream &inf){
  std::vector<ServerContext> server;
  std::string line;

	while (std::getline(inf, line)){
    std::stringstream ss(line);
    std::string key;
    std::string value;
		ss >> key;
    if (key.empty())
      continue ;
    ss >> value;
    if (key + value == "server{" && ss.get() == std::char_traits<char>::eof())
      ServerParser::parse_server(line);
    else{
      std::cerr << "Syntax Error: not server" << std::endl;
      throw std::exception();
    }

	}
}