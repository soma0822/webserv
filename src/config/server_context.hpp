#ifndef SERVER_CONTEXT_HPP
#define SERVER_CONTEXT_HPP

#include <iostream>
#include <map>
#include <vector>

#include "location_context.hpp"

#define LOCATION "\033[1;32m"
#define SERVER "\033[1;33m"

class ServerContext {
 public:
  ServerContext();
  ServerContext(const ServerContext &other);
  ServerContext &operator=(const ServerContext &other);

  const std::string &GetIp() const;
  const std::string &GetRoot() const;
  const std::vector<std::string> &GetIndex() const;
  const std::vector<std::string> &GetPort() const;
  const std::vector<std::string> &GetServerName() const;
  const std::map<std::string, std::string> &GetErrorPage() const;
  const std::map<std::string, LocationContext> &GetLocation() const;

  void SetIp(const std::string &);
  void SetRoot(const std::string &);
  void AddIndex(const std::string &);
  void AddPort(const std::string &);
  void AddServerName(const std::string &);
  void AddErrorPage(const std::string &, const std::string &);
  void AddLocation(const std::string &, const LocationContext &);

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
