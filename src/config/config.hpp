#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <stdexcept>
#include <vector>

#include "config_parser.hpp"
#include "server_context.hpp"

#define DEFAULTCONF "conf/default.conf"

class Config {
 public:
  Config();
  Config(const std::string &);
  Config(const Config &other);
  ~Config();
  Config &operator=(const Config &other);
  void ParseFile();
  const std::vector<ServerContext> &GetServer() const;

 private:
  std::string file_;
  std::vector<ServerContext> server_;
};

std::ostream &operator<<(std::ostream &os, Config &config);
std::ostream &operator<<(std::ostream &os, const Config &config);

#endif
