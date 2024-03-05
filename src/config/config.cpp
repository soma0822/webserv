#include "config.hpp"

std::vector<ServerContext> Config::server_;

void Config::Clear() { server_.clear(); }

void Config::AddServer(const ServerContext &server) {
  server_.push_back(server);
}

const std::vector<ServerContext> &Config::GetServer() { return server_; }

const ServerContext &Config::SearchServer(const std::string &port,
                                          const std::string &server_name) {
  long ret = -1;
  for (unsigned int i = 0; i < server_.size(); ++i) {
    if (server_.at(i).HavePort(port) &&
        (server_.at(i).HaveServerName(server_name) || ret == -1))
      ret = i;
  }
  // portが一致しているものが一つもないはずはない。あったら投げるほうが間違っている。
  if (ret == -1) throw std::exception();
  return server_.at(ret);
}
