#ifndef SERVER_CONTEXT_HPP
#define SERVER_CONTEXT_HPP

#include <iostream>
#include <map>
#include <vector>
#include "location_context.hpp"

class ServerContext{
public:
  ServerContext();
  ServerContext(const ServerContext &other);
  ServerContext &operator=(const ServerContext &other);
  const std::string &get_ip() const;
  const std::vector<std::string> &get_port() const;
  const std::vector<std::string> &get_server_name() const;
  const std::map<std::string, LocationContext> &get_location() const;
  void set_ip(const std::string &);
  void add_port(const std::string &);
  void add_server_name(const std::string &);
  void add_location(const std::string &, const LocationContext &);

private:
  std::string ip_;
  std::vector<std::string> port_;
  std::vector<std::string> server_name_;
  std::map<std::string, LocationContext> location_;
};

#endif