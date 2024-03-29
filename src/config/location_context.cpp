#include "location_context.hpp"

LocationContext::LocationContext()
    : can_auto_index_(false), limit_client_body_bytes_(1000) {
  allow_method_["GET"] = false;
  allow_method_["POST"] = false;
  allow_method_["DELETE"] = false;
}
LocationContext::LocationContext(const LocationContext &other) {
  *this = other;
}
LocationContext &LocationContext::operator=(const LocationContext &other) {
  if (this == &other) {
    return *this;
  }
  can_auto_index_ = other.can_auto_index_;
  limit_client_body_bytes_ = other.limit_client_body_bytes_;
  path_ = other.path_;
  return_ = other.return_;
  alias_ = other.alias_;
  root_ = other.root_;
  index_ = other.index_;
  cgi_extension_ = other.cgi_extension_;
  allow_method_ = other.allow_method_;
  return *this;
}
// ゲッター
bool LocationContext::GetCanAutoIndex() const { return can_auto_index_; }
int LocationContext::GetLimitClientBody() const {
  return limit_client_body_bytes_;
}
const std::string &LocationContext::GetPath() const { return path_; }
const std::string &LocationContext::GetReturn() const { return return_; }
const std::string &LocationContext::GetAlias() const { return alias_; }
const std::string &LocationContext::GetRoot() const { return root_; }
const std::string &LocationContext::GetIndex() const { return index_; }
const std::vector<std::string> &LocationContext::GetCgiExtension() const {
  return cgi_extension_;
}
bool LocationContext::IsValidCgiExtension(const std::string &extension) const {
  return std::find(cgi_extension_.begin(), cgi_extension_.end(), extension) !=
         cgi_extension_.end();
}
const std::map<std::string, bool> &LocationContext::GetAllowMethod() const {
  return allow_method_;
}
bool LocationContext::IsAllowedMethod(const std::string &key) const {
  std::map<std::string, bool>::const_iterator it = allow_method_.find(key);
  return it->second == true;
}
// セッター
void LocationContext::SetCanAutoIndex(bool can_auto_index) {
  can_auto_index_ = can_auto_index;
}
void LocationContext::SetLimitClientBody(int limit_client_body_bytes) {
  limit_client_body_bytes_ = limit_client_body_bytes;
}
void LocationContext::SetPath(const std::string &path) { path_ = path; }
void LocationContext::SetReturn(const std::string &ret) { return_ = ret; }
void LocationContext::SetAlias(const std::string &alias) { alias_ = alias; }
void LocationContext::SetRoot(const std::string &root) { root_ = root; }
void LocationContext::SetIndex(const std::string &index) { index_ = index; }
void LocationContext::AddCgiExtension(const std::string &cgi_extension) {
  if (cgi_extension_.end() !=
      std::find(cgi_extension_.begin(), cgi_extension_.end(), cgi_extension))
    throw std::invalid_argument(
        "cgi_extensionで同じものが複数指定されています");
  cgi_extension_.push_back(cgi_extension);
}
void LocationContext::AddAllowMethod(const std::string &key) {
  if (allow_method_[key] == true)
    throw std::invalid_argument("allow_methodで同じものが複数指定されています");
  allow_method_[key] = true;
}

// 出力
std::ostream &operator<<(std::ostream &os, LocationContext &obj) {
  os << "path: " << obj.GetPath();
  os << "\n index: ";
  os << obj.GetIndex();
  os << "\n root: " << obj.GetRoot();
  const std::map<std::string, bool> allow_method = obj.GetAllowMethod();
  for (std::map<std::string, bool>::const_iterator it = allow_method.begin();
       it != allow_method.end(); ++it) {
    os << (it->second ? it->first + " " : "");
  }
  os << "\n cgiExtension: ";
  for (std::vector<std::string>::const_iterator it =
           obj.GetCgiExtension().begin();
       it != obj.GetCgiExtension().end(); ++it) {
    os << *it << " ";
  }
  os << "\n autoindex: " << (obj.GetCanAutoIndex() == true ? "on" : "off");
  os << "\n return: "
     << (!obj.GetReturn().empty() ? obj.GetReturn() : "no set");
  os << "\n alias: " << (!obj.GetAlias().empty() ? obj.GetAlias() : "no set");
  os << "\n limit client body: " << obj.GetLimitClientBody();
  return os;
}

std::ostream &operator<<(std::ostream &os, const LocationContext &obj) {
  os << " path: " << obj.GetPath();
  os << "\n index: ";
  os << obj.GetIndex();
  os << "\n root: " << (obj.GetRoot().empty() ? "no set" : obj.GetRoot());
  os << "\n allow method: ";
  const std::map<std::string, bool> allow_method = obj.GetAllowMethod();
  for (std::map<std::string, bool>::const_iterator it = allow_method.begin();
       it != allow_method.end(); ++it) {
    os << (it->second ? it->first + " " : "");
  }
  os << "\n cgiExtension: ";
  if (obj.GetCgiExtension().size() == 0)
    os << "no set";
  else {
    for (std::vector<std::string>::const_iterator it =
             obj.GetCgiExtension().begin();
         it != obj.GetCgiExtension().end(); ++it)
      os << *it << " ";
  }
  os << "\n autoindex: " << (obj.GetCanAutoIndex() == true ? "on" : "off");
  os << "\n return: "
     << (!obj.GetReturn().empty() ? obj.GetReturn() : "no set");
  os << "\n alias: " << (!obj.GetAlias().empty() ? obj.GetAlias() : "no set");
  os << "\n limit client body: " << obj.GetLimitClientBody();
  return os;
}
