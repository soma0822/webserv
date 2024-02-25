#include "server_context.hpp"

ServerContext::ServerContext() : ip_("") {}

ServerContext::ServerContext(const ServerContext &other) : ip_(other.ip_) {
  *this = other;
}

ServerContext &ServerContext::operator=(const ServerContext &other) {
  if (this == &other) {
    return *this;
  }
  ip_ = other.ip_;
  root_ = other.root_;
  index_ = other.index_;
  port_ = other.port_;
  server_name_ = other.server_name_;
  error_page_ = other.error_page_;
  location_ = other.location_;
  return *this;
}

// ゲッター
const std::string &ServerContext::GetIp() const { return ip_; }
const std::string &ServerContext::GetRoot() const { return root_; }
const std::vector<std::string> &ServerContext::GetIndex() const {
  return index_;
}
const std::vector<std::string> &ServerContext::GetPort() const { return port_; }
const std::vector<std::string> &ServerContext::GetServerName() const {
  return server_name_;
}
const std::map<std::string, std::string> &ServerContext::GetErrorPage() const {
  return error_page_;
}
const std::map<std::string, LocationContext> &ServerContext::GetLocation()
    const {
  return location_;
}

// セッター
void ServerContext::SetIp(const std::string &ip) {
  if (ip_ == "") ip_ = ip;
}
void ServerContext::SetRoot(const std::string &root) { root_ = root; }
void ServerContext::AddIndex(const std::string &index) {
  index_.push_back(index);
}
void ServerContext::AddPort(const std::string &port) { port_.push_back(port); }
void ServerContext::AddServerName(const std::string &server_name) {
  server_name_.push_back(server_name);
}
void ServerContext::AddErrorPage(const std::string &key,
                                 const std::string &value) {
  std::map<std::string, std::string>::iterator it = error_page_.find(key);
  if (it == error_page_.end()) error_page_[key] = value;
}
void ServerContext::AddLocation(const std::string &key,
                                const LocationContext &value) {
  std::map<std::string, LocationContext>::iterator it = location_.find(key);
  if (it == location_.end()) location_[key] = value;
}

std::ostream &operator<<(std::ostream &os, ServerContext &obj) {
  os << SERVER;
  std::vector<std::string> tmp = obj.GetPort();
  os << "port: ";
  if (tmp.size() == 0)
    os << "no set";
  else {
    for (std::vector<std::string>::const_iterator it = tmp.begin();
         it != tmp.end(); it++)
      os << *it << " ";
  }
  tmp = obj.GetServerName();
  os << "\nserver_name: ";
  if (tmp.size() == 0)
    os << "no set";
  else {
    for (std::vector<std::string>::const_iterator it = tmp.begin();
         it != tmp.end(); it++)
      os << *it << " ";
  }
  tmp = obj.GetIndex();
  os << "\nindex: ";
  if (tmp.size() == 0)
    os << "no set";
  else {
    for (std::vector<std::string>::const_iterator it = tmp.begin();
         it != tmp.end(); it++)
      os << *it << " ";
  }
  os << "\nerror page: ";
  if (obj.GetErrorPage().size() == 0)
    os << "no set";
  else {
    for (std::map<std::string, std::string>::const_iterator it =
             obj.GetErrorPage().begin();
         it != obj.GetErrorPage().end(); it++)
      os << it->first << "[" << it->second << "]"
         << "    ";
  }
  os << "\nroot: " << (obj.GetRoot().empty() ? "no set" : obj.GetRoot());
  // os << "\nport: " << obj.getPort();
  os << "\nip: " << (obj.GetIp().empty() ? "no set" : obj.GetIp());
  for (std::map<std::string, LocationContext>::const_iterator it =
           obj.GetLocation().begin();
       it != obj.GetLocation().end(); it++) {
    os << LOCATION << "\nLocation: ";
    os << it->first << "\n";
    os << it->second << "\n";
    os << "}\n";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, const ServerContext &obj) {
  os << SERVER;
  std::vector<std::string> tmp = obj.GetPort();
  os << "port: ";
  if (tmp.size() == 0)
    os << "no set";
  else {
    for (std::vector<std::string>::const_iterator it = tmp.begin();
         it != tmp.end(); it++)
      os << *it << " ";
  }
  tmp = obj.GetServerName();
  os << "\nserver_name: ";
  if (tmp.size() == 0)
    os << "no set";
  else {
    for (std::vector<std::string>::const_iterator it = tmp.begin();
         it != tmp.end(); it++)
      os << *it << " ";
  }
  tmp = obj.GetIndex();
  os << "\nindex: ";
  if (tmp.size() == 0)
    os << "no set";
  else {
    for (std::vector<std::string>::const_iterator it = tmp.begin();
         it != tmp.end(); it++)
      os << *it << " ";
  }
  os << "\nerror page: ";
  if (obj.GetErrorPage().size() == 0)
    os << "no set";
  else {
    for (std::map<std::string, std::string>::const_iterator it =
             obj.GetErrorPage().begin();
         it != obj.GetErrorPage().end(); it++)
      os << it->first << "[" << it->second << "]"
         << "    ";
  }
  os << "\nroot: " << (obj.GetRoot().empty() ? "no set" : obj.GetRoot());
  // os << "\nport: " << obj.getPort();
  os << "\nip: " << (obj.GetIp().empty() ? "no set" : obj.GetIp());
  for (std::map<std::string, LocationContext>::const_iterator it =
           obj.GetLocation().begin();
       it != obj.GetLocation().end(); it++) {
    os << LOCATION << "\nLocation: ";
    os << it->first << "\n";
    os << it->second << "\n";
    os << "}\n";
  }
  return os;
}