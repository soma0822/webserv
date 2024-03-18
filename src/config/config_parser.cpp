#include "config_parser.hpp"

const std::string ConfigParser::default_file_ = "./conf/default.conf";

Config ConfigParser::Parse(const std::string &file) {
  ServerParser::ClearParsedPair();
  Config config;
  std::vector<ServerContext> server;
  std::string line;
  std::ifstream inf(file.c_str());
  if (!inf.is_open()) {
    throw std::invalid_argument("ファイルが開けませんでした: " + file);
  }
  while (std::getline(inf, line)) {
    std::stringstream ss(line);
    std::string key;
    std::string value;
    ss >> key;
    if (key.empty()) continue;
    if (key == "error_page"){
      std::vector<std::string> value;
      std::string tmp;
      while (ss >> tmp) value.push_back(tmp);
      if (ConfigParser::ParseErrorPage(value, config) == false)
        throw std::invalid_argument("無効なerror_page: " + line);
      continue;
    }
    ss >> value;
    if (key + value == "server{" && ss.get() == std::char_traits<char>::eof())
      config.AddServer(ServerParser::ParseServer(inf));
    else {
      throw std::invalid_argument("無効なキー: " + line);
    }
  }
  if (ServerParser::UniqueListen() == false)
    throw std::invalid_argument("同じポート、同じipで複数設定されています");
  if (config.GetServer().empty()) {
    throw std::invalid_argument("serverがありません");
  }
  return config;
}

bool ConfigParser::ParseErrorPage(const std::vector<std::string> &value,
                                    Config &config) {
  if (value.size() < 2) return false;
  for (unsigned int i = 0; i < value.size() - 1; ++i) {
    if (validation::IsNumber(value.at(i)) == false) return false;
    config.AddErrorPage(value.at(i), *(value.end() - 1));
  }
  return true;
}
