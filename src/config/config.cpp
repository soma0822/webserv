#include "config.hpp"

Config::Config() {}

Config::Config(const Config &other) : server_(other.server_) {}

Config::~Config() {}

Config &Config::operator=(const Config &other) {
  (void)other;
  return *this;
}

void Config::AddServer(const ServerContext &server) {
  server_.push_back(server);
}

const std::vector<ServerContext> &Config::GetServer() const { return server_; }

const IServerContext &Config::SearchServer(
    const std::string &port, const std::string &ip,
    const std::string &server_name) const {
  std::vector<ServerContext>::const_iterator ans = server_.end();
  std::vector<ServerContext>::const_iterator it = server_.begin();
  for (; it != server_.end(); ++it) {
    if (it->HavePort(port) && ip == it->GetIp() &&
        (it->HaveServerName(server_name) || ans == server_.end()))
      ans = it;
  }
  return *ans;
}
