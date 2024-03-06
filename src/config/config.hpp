#ifndef WEBSERV_SRC_CONFIG_CONFIG_HPP
#define WEBSERV_SRC_CONFIG_CONFIG_HPP

#include <iostream>
#include <stdexcept>
#include <vector>

#include "config_parser.hpp"
#include "server_context.hpp"

class ConfigParser;
class Config {
 public:
  static void Clear();
  static void AddServer(const ServerContext &server);
  static const std::vector<ServerContext> &GetServer();
  static const ServerContext &SearchServer(const std::string &port,
                                           const std::string &ip, const std::string &server_name);

 private:
  Config();
  Config(const Config &other);
  ~Config();
  static std::vector<ServerContext> server_;
};

#endif
