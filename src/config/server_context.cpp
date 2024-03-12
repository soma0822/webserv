#include "server_context.hpp"

ServerContext::ServerContext() {}

ServerContext::ServerContext(const ServerContext &other) { *this = other; }

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

bool ServerContext::HavePort(const std::string &port) const {
  return (port_ == port);
}

bool ServerContext::HaveServerName(const std::string &server_name) const {
  return (server_name_ == server_name);
}

// ポートを指定しないサーバー設定はエラー。他はなくてもいい。portについてもデフォルト決めるのもあり
bool ServerContext::IsValidContext() const {
  if (port_.size() == 0)
    return false;
  else
    return true;
}

Result<LocationContext, std::string> ServerContext::SearchLocation(
    const std::string &path) const {
  if (location_.empty()) {
    return Err("locationが設定されていません");
  }
  std::map<std::string, LocationContext>::const_iterator it = location_.begin();
  std::map<std::string, LocationContext>::const_iterator ret = location_.end();
  long unsigned int ret_len = 0;
  for (; it != location_.end(); ++it) {
    if (it->first[0] == '=' && it->first.substr(2) == path) {
      return Ok(it->second);
    } else {
      if (path.find(it->first) == 0 && ret_len < it->first.length() &&
          (path.length() == it->first.length() ||
           path[it->first.length()] == '/')) {
        ret_len = it->first.length();
        ret = it;
      }
    }
  }
  if (ret == location_.end()) {
    return Err("locationが見つかりません");
  }
  return Ok(ret->second);
}

// ゲッター
const std::string &ServerContext::GetIp() const { return ip_; }
const std::string &ServerContext::GetRoot() const { return root_; }
const std::string &ServerContext::GetIndex() const { return index_; }
const std::string &ServerContext::GetPort() const { return port_; }
const std::string &ServerContext::GetServerName() const { return server_name_; }
const std::map<std::string, std::string> &ServerContext::GetErrorPage() const {
  return error_page_;
}
const std::map<std::string, LocationContext> &ServerContext::GetLocation()
    const {
  return location_;
}

// セッター
void ServerContext::SetIp(const std::string &ip) { ip_ = ip; }
void ServerContext::SetRoot(const std::string &root) { root_ = root; }
void ServerContext::SetIndex(const std::string &index) { index_ = index; }
void ServerContext::SetPort(const std::string &port) { port_ = port; }
void ServerContext::SetServerName(const std::string &server_name) {
  server_name_ = server_name;
}
void ServerContext::AddErrorPage(const std::string &key,
                                 const std::string &value) {
  std::map<std::string, std::string>::iterator it = error_page_.find(key);
  if (it != error_page_.end())
    throw std::invalid_argument("error_pageで同じものが複数指定されています");
  error_page_[key] = value;
}
void ServerContext::AddLocation(const std::string &key,
                                const LocationContext &value) {
  std::map<std::string, LocationContext>::iterator it = location_.find(key);
  if (it != location_.end())
    throw std::invalid_argument("locationで同じものが複数指定されています");
  location_[key] = value;
}

std::ostream &operator<<(std::ostream &os, ServerContext &obj) {
  os << SERVER;
  std::vector<std::string> tmp;
  os << "port: ";
  os << obj.GetPort();
  os << "\nserver_name: ";
  os << obj.GetServerName();
  os << "\nindex: ";
  os << obj.GetIndex();
  os << "\nerror page: ";
  if (obj.GetErrorPage().size() == 0)
    os << "no set";
  else {
    for (std::map<std::string, std::string>::const_iterator it =
             obj.GetErrorPage().begin();
         it != obj.GetErrorPage().end(); ++it)
      os << it->first << "[" << it->second << "]"
         << "    ";
  }
  os << "\nroot: " << (obj.GetRoot().empty() ? "no set" : obj.GetRoot());
  os << "\nip: " << (obj.GetIp().empty() ? "no set" : obj.GetIp());
  for (std::map<std::string, LocationContext>::const_iterator it =
           obj.GetLocation().begin();
       it != obj.GetLocation().end(); ++it) {
    os << LOCATION << "\nLocation: ";
    os << it->first << "\n";
    os << it->second << "\n";
    os << "}\n";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, const ServerContext &obj) {
  os << SERVER;
  std::vector<std::string> tmp;
  os << "port: ";
  os << obj.GetPort();
  os << "\nserver_name: ";
  os << obj.GetServerName();
  os << "\nindex: ";
  os << obj.GetIndex();
  os << "\nerror page: ";
  if (obj.GetErrorPage().size() == 0)
    os << "no set";
  else {
    for (std::map<std::string, std::string>::const_iterator it =
             obj.GetErrorPage().begin();
         it != obj.GetErrorPage().end(); ++it)
      os << it->first << "[" << it->second << "]"
         << "    ";
  }
  os << "\nroot: " << (obj.GetRoot().empty() ? "no set" : obj.GetRoot());
  os << "\nip: " << (obj.GetIp().empty() ? "no set" : obj.GetIp());
  for (std::map<std::string, LocationContext>::const_iterator it =
           obj.GetLocation().begin();
       it != obj.GetLocation().end(); ++it) {
    os << LOCATION << "\nLocation: ";
    os << it->first << "\n";
    os << it->second << "\n";
    os << "}\n";
  }
  return os;
}
