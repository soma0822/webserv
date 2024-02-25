#include "config_parser.hpp"

Config Parse(const std::string &file) {
  Config config;
  std::vector<ServerContext> server;
  std::string line;
  std::ifstream inf(file);
  if (!inf.is_open()) {
    throw std::invalid_argument("File could not open: " + file);
  }
  while (std::getline(inf, line)) {
    std::stringstream ss(line);
    std::string key;
    std::string value;
    ss >> key;
    if (key.empty()) continue;
    ss >> value;
    if (key + value == "server{" && ss.get() == std::char_traits<char>::eof())
      config.addServer(ServerParser::ParseServer(inf));
    else {
      throw std::invalid_argument("Invalid key: " + line);
    }
  }
  return config;
}
