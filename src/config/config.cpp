#include "config.hpp"

std::vector<ServerContext> Config::server_;

void Config::AddServer(const ServerContext &server) {
  server_.push_back(server);
}

const std::vector<ServerContext> &Config::GetServer() { return server_; }