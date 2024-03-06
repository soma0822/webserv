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
  ~Config();
  Config(const Config &other);
  void AddServer(const ServerContext &server);
  const std::vector<ServerContext> &GetServer() const;
  const ServerContext &SearchServer(const std::string &port,
                                    const std::string &ip,
                                    const std::string &server_name) const;

 private:
  Config &operator=(const Config &other);
  std::vector<ServerContext> server_;
};

#endif
