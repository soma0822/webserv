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
  if (port_.end() == std::find(port_.begin(), port_.end(), port)) return false;
  return true;
}

bool ServerContext::HaveServerName(const std::string &server_name) const {
  if (server_name_.end() ==
      std::find(server_name_.begin(), server_name_.end(), server_name))
    return false;
  return true;
}

// ポートを指定しないサーバー設定はエラー。他はなくてもいい。portについてもデフォルト決めるのもあり
bool ServerContext::IsValidContext() const {
  if (port_.size() == 0)
    return false;
  else
    return true;
}

const LocationContext &ServerContext::SearchLocation(
    const std::string &path) const {
  std::map<std::string, LocationContext>::const_iterator it = location_.begin();
  std::map<std::string, LocationContext>::const_iterator ret = location_.end();
  int ret_len = 0;
  for (; it != location_.end(); it++) {
    if (it->first[0] == '=' && it->first.substr(2) == path) {
      return it->second;
    } else {
      if (path.find(it->first) == 0 && ret_len < it->first.length() &&
          (path.length() == it->first.length() ||
           path[it->first.length()] == '/')) {
        ret_len = it->first.length();
        ret = it;
      }
    }
  }
  return ret->second;
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
void ServerContext::SetIp(const std::string &ip) { ip_ = ip; }
void ServerContext::SetRoot(const std::string &root) { root_ = root; }
void ServerContext::AddIndex(const std::string &index) {
  if (index_.end() != std::find(index_.begin(), index_.end(), index))
    throw std::invalid_argument("indexで同じものが複数指定されています");
  index_.push_back(index);
}
void ServerContext::AddPort(const std::string &port) {
  if (port_.end() != std::find(port_.begin(), port_.end(), port))
    throw std::invalid_argument("portで同じものが複数指定されています");
  port_.push_back(port);
}
void ServerContext::AddServerName(const std::string &server_name) {
  if (server_name_.end() !=
      std::find(server_name_.begin(), server_name_.end(), server_name))
    throw std::invalid_argument("server_nameで同じものが複数指定されています");
  server_name_.push_back(server_name);
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
  std::vector<std::string> tmp = obj.GetPort();
  os << "port: ";
  if (tmp.size() == 0)
    os << "no set";
  else {
    for (std::vector<std::string>::const_iterator it = tmp.begin();
         it != tmp.end(); ++it)
      os << *it << " ";
  }
  tmp = obj.GetServerName();
  os << "\nserver_name: ";
  if (tmp.size() == 0)
    os << "no set";
  else {
    for (std::vector<std::string>::const_iterator it = tmp.begin();
         it != tmp.end(); ++it)
      os << *it << " ";
  }
  tmp = obj.GetIndex();
  os << "\nindex: ";
  if (tmp.size() == 0)
    os << "no set";
  else {
    for (std::vector<std::string>::const_iterator it = tmp.begin();
         it != tmp.end(); ++it)
      os << *it << " ";
  }
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
  // os << "\nport: " << obj.getPort();
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
  std::vector<std::string> tmp = obj.GetPort();
  os << "port: ";
  if (tmp.size() == 0)
    os << "no set";
  else {
    for (std::vector<std::string>::const_iterator it = tmp.begin();
         it != tmp.end(); ++it)
      os << *it << " ";
  }
  tmp = obj.GetServerName();
  os << "\nserver_name: ";
  if (tmp.size() == 0)
    os << "no set";
  else {
    for (std::vector<std::string>::const_iterator it = tmp.begin();
         it != tmp.end(); ++it)
      os << *it << " ";
  }
  tmp = obj.GetIndex();
  os << "\nindex: ";
  if (tmp.size() == 0)
    os << "no set";
  else {
    for (std::vector<std::string>::const_iterator it = tmp.begin();
         it != tmp.end(); ++it)
      os << *it << " ";
  }
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
  // os << "\nport: " << obj.getPort();
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
