#include "config.hpp"

Config::Config() {}

Config::Config(const Config &other) { *this = other; }

Config::~Config() {}

Config &Config::operator=(const Config &other) {
  if (this == &other) {
    return *this;
  }
  server_ = other.server_;
  return *this;
}

void Config::addServer(const ServerContext &server) {
  server_.push_back(server);
}

const std::vector<ServerContext> &Config::GetServer() const { return server_; }

std::ostream &operator<<(std::ostream &os, Config &config) {
  for (std::vector<ServerContext>::const_iterator it =
           config.GetServer().begin();
       it != config.GetServer().end(); it++) {
    os << *it << std::endl;
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, const Config &config) {
  for (std::vector<ServerContext>::const_iterator it =
           config.GetServer().begin();
       it != config.GetServer().end(); it++) {
    os << *it << std::endl;
  }
  return os;
}
