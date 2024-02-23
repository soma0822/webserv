#include "config_parser.hpp"

std::vector<ServerContext> ConfigParser::parse(std::ifstream &inf) {
  std::vector<ServerContext> server;
  std::string line;

  while (std::getline(inf, line)) {
    std::stringstream ss(line);
    std::string key;
    std::string value;
    ss >> key;
    if (key.empty()) continue;
    ss >> value;
    if (key + value == "server{" && ss.get() == std::char_traits<char>::eof())
      server.push_back(ServerParser::parse_server(inf));
    else {
      throw std::invalid_argument("Invalid key: " + line);
    }
  }
  return server;
}
