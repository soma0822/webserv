#ifndef WEBSERV_SRC_CONFIG_SERVER_CONTEXT_HPP
#define WEBSERV_SRC_CONFIG_SERVER_CONTEXT_HPP

#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#include "location_context.hpp"

#define LOCATION "\033[1;32m"
#define SERVER "\033[1;33m"

class ServerContext {
 public:
  ServerContext();
  ServerContext(const ServerContext &other);
  ServerContext &operator=(const ServerContext &other);

  bool HavePort(const std::string &port) const;
  bool HaveServerName(const std::string &server_name) const;
  bool IsValidContext() const;
  // ゲッター
  const std::string &GetIp() const;
  const std::string &GetRoot() const;
  const std::vector<std::string> &GetIndex() const;
  const std::vector<std::string> &GetPort() const;
  const std::vector<std::string> &GetServerName() const;
  const std::map<std::string, std::string> &GetErrorPage() const;
  const std::map<std::string, LocationContext> &GetLocation() const;
  // セッター
  void SetIp(const std::string &ip);
  void SetRoot(const std::string &root);
  void AddIndex(const std::string &index);
  void AddPort(const std::string &port);
  void AddServerName(const std::string &server_name);
  void AddErrorPage(const std::string &key, const std::string &value);
  void AddLocation(const std::string &key, const LocationContext &value);

 private:
  std::string ip_;
  std::string root_;
  std::vector<std::string> index_;
  std::vector<std::string> port_;
  std::vector<std::string> server_name_;
  std::map<std::string, std::string> error_page_;
  std::map<std::string, LocationContext> location_;
};

std::ostream &operator<<(std::ostream &os, ServerContext &obj);
std::ostream &operator<<(std::ostream &os, const ServerContext &obj);

#endif
