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

class IServerContext {
 public:
  virtual ~IServerContext() {}
  virtual bool HavePort(const std::string &port) const = 0;
  virtual bool HaveServerName(const std::string &server_name) const = 0;
  virtual bool IsValidContext() const = 0;
  // ゲッター
  virtual const std::string &GetIp() const = 0;
  virtual const std::string &GetRoot() const = 0;
  virtual const std::string &GetIndex() const = 0;
  virtual const std::string &GetPort() const = 0;
  virtual const std::string &GetServerName() const = 0;
  virtual const std::map<std::string, std::string> &GetErrorPage() const = 0;
  virtual const std::map<std::string, LocationContext> &GetLocation() const = 0;
  // セッター
  virtual void SetIp(const std::string &ip) = 0;
  virtual void SetRoot(const std::string &root) = 0;
  virtual void SetIndex(const std::string &index) = 0;
  virtual void SetPort(const std::string &port) = 0;
  virtual void SetServerName(const std::string &server_name) = 0;
  virtual void AddErrorPage(const std::string &key,
                            const std::string &value) = 0;
  virtual void AddLocation(const std::string &key,
                           const LocationContext &value) = 0;
};

class ServerContext : public IServerContext {
 public:
  ServerContext();
  ServerContext(const ServerContext &other);
  ServerContext &operator=(const ServerContext &other);

  bool HavePort(const std::string &port) const;
  bool HaveServerName(const std::string &server_name) const;
  bool IsValidContext() const;
  const LocationContext &SearchLocation(const std::string &path) const;
  // ゲッター
  const std::string &GetIp() const;
  const std::string &GetRoot() const;
  const std::string &GetIndex() const;
  const std::string &GetPort() const;
  const std::string &GetServerName() const;
  const std::map<std::string, std::string> &GetErrorPage() const;
  const std::map<std::string, LocationContext> &GetLocation() const;
  // セッター
  void SetIp(const std::string &ip);
  void SetRoot(const std::string &root);
  void SetIndex(const std::string &index);
  void SetPort(const std::string &port);
  void SetServerName(const std::string &server_name);
  void AddErrorPage(const std::string &key, const std::string &value);
  void AddLocation(const std::string &key, const LocationContext &value);

 private:
  std::string ip_;
  std::string root_;
  std::string index_;
  std::string port_;
  std::string server_name_;
  std::map<std::string, std::string> error_page_;
  std::map<std::string, LocationContext> location_;
};

std::ostream &operator<<(std::ostream &os, ServerContext &obj);
std::ostream &operator<<(std::ostream &os, const ServerContext &obj);

#endif
