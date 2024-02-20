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
  const std::string &get_root() const;
  const std::vector<std::string> &get_index() const;
  const std::vector<std::string> &get_port() const;
  const std::vector<std::string> &get_server_name() const;
  const std::map<std::string, std::string> &get_error_page() const;
  const std::map<std::string, LocationContext> &get_location() const;

  void set_ip(const std::string &);
  void set_root(const std::string &);
  void add_index(const std::string &);
  void add_port(const std::string &);
  void add_server_name(const std::string &);
  void add_error_page(const std::string &, const std::string &);
  void add_location(const std::string &, const LocationContext &);

private:
  std::string ip_;
  std::string	root_;
  std::vector<std::string> index_;
  std::vector<std::string> port_;
  std::vector<std::string> server_name_;
  std::map<std::string, std::string>	error_page_;
  
};

#endif