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
  Config();
  Config(const Config &other);
  ~Config();
  Config &operator=(const Config &other);
  const std::vector<ServerContext> &GetServer() const;
  void addServer(const ServerContext &);

 private:
  std::vector<ServerContext> server_;
};

std::ostream &operator<<(std::ostream &os, Config &config);
std::ostream &operator<<(std::ostream &os, const Config &config);

#endif
