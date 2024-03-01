#include "config.hpp"

std::vector<ServerContext> Config::server_;

void Config::Clear() { server_.clear(); }

void Config::AddServer(const ServerContext &server) {
  server_.push_back(server);
}

const std::vector<ServerContext> &Config::GetServer() { return server_; }
