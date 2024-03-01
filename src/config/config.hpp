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
  static void AddServer(const ServerContext &);
  static const std::vector<ServerContext> &GetServer();

 private:
  Config();
  Config(const Config &other);
  ~Config();
  static std::vector<ServerContext> server_;
};

#endif
