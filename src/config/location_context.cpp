#include "location_context.hpp"

LocationContext::LocationContext()
    : can_auto_index_(false),
      limit_client_body_(INT_MAX),
      return_(""),
      alias_(""),
      root_("") {
  allow_method_["GET"] = false;
  allow_method_["POST"] = false;
  allow_method_["DELETE"] = false;
}
LocationContext::LocationContext(const LocationContext& other) {
  *this = other;
}
LocationContext& LocationContext::operator=(const LocationContext& other) {
  if (this == &other) {
    return *this;
  }
  can_auto_index_ = other.can_auto_index_;
  limit_client_body_ = other.limit_client_body_;
  return_ = other.return_;
  alias_ = other.alias_;
  root_ = other.root_;
  index_ = other.index_;
  cgi_path_ = other.cgi_path_;
  cgi_extention_ = other.cgi_extention_;
  allow_method_ = other.allow_method_;
  error_page_ = other.error_page_;
  return *this;
}
// ゲッター
bool LocationContext::GetCnaAutoIndex() const { return can_auto_index_; }
int LocationContext::GetLimitClientBody() const { return limit_client_body_; }
const std::string& LocationContext::GetReturn() const { return return_; }
const std::string& LocationContext::GetAlias() const { return alias_; }
const std::string& LocationContext::GetRoot() const { return root_; }
const std::vector<std::string>& LocationContext::GetIndex() const {
  return index_;
}
const std::vector<std::string>& LocationContext::GetCgiPath() const {
  return cgi_path_;
}
const std::vector<std::string>& LocationContext::GetCgiExtention() const {
  return cgi_extention_;
}
const std::map<std::string, bool>& LocationContext::GetAllowMethod() const {
  return allow_method_;
}
const std::map<std::string, std::string>& LocationContext::GetErrorPage()
    const {
  return error_page_;
}
// セッター
void LocationContext::SetCanAutoIndex(bool can_auto_index) {
  can_auto_index_ = can_auto_index;
}
void LocationContext::SetLimitClientBody(int limit_client_body) {
  limit_client_body_ = limit_client_body;
}
void LocationContext::SetReturn(const std::string& ret) { return_ = ret; }
void LocationContext::SetAlias(const std::string& alias) { alias_ = alias; }
void LocationContext::SetRoot(const std::string& root) { root_ = root; }
void LocationContext::AddIndex(const std::string& index) {
  index_.push_back(index);
}
void LocationContext::AddCgiPath(const std::string& cgi_path) {
  cgi_path_.push_back(cgi_path);
}
void LocationContext::AddCgiExtention(const std::string& cgi_extention) {
  cgi_extention_.push_back(cgi_extention);
}
void LocationContext::AddAllowMethod(const std::string& key) {
  allow_method_[key] = true;
}
void LocationContext::AddErrorPage(const std::string& key,
                                   const std::string& value) {
  std::map<std::string, std::string>::iterator it = error_page_.find(key);
  if (it == error_page_.end()) error_page_[key] = value;
}

// 出力
std::ostream& operator<<(std::ostream& os, LocationContext& obj) {
  os << "index: ";
  for (std::vector<std::string>::const_iterator it = obj.GetIndex().begin();
       it != obj.GetIndex().end(); it++) {
    os << *it << " ";
  }
  os << "\nerror page: ";
  for (std::map<std::string, std::string>::const_iterator it =
           obj.GetErrorPage().begin();
       it != obj.GetErrorPage().end(); it++) {
    os << it->first << "[" << it->second << "]"
       << "    ";
  }
  os << "\nroot: " << obj.GetRoot();
  const std::map<std::string, bool> allow_method = obj.GetAllowMethod();
  for (std::map<std::string, bool>::const_iterator it = allow_method.begin();
       it != allow_method.end(); ++it) {
    os << (it->second ? it->first + " " : "");
  }
  os << "\ncgiPath: ";
  for (std::vector<std::string>::const_iterator it = obj.GetCgiPath().begin();
       it != obj.GetCgiPath().end(); it++) {
    os << *it << " ";
  }
  os << "\ncgiExtention: ";
  for (std::vector<std::string>::const_iterator it =
           obj.GetCgiExtention().begin();
       it != obj.GetCgiExtention().end(); it++) {
    os << *it << " ";
  }
  os << "\nautoindex: " << (obj.GetCnaAutoIndex() == true ? "on" : "off");
  os << "\nreturn: " << (!obj.GetReturn().empty() ? obj.GetReturn() : "no set");
  os << "\nalias: " << (!obj.GetAlias().empty() ? obj.GetAlias() : "no set");
  os << "\nlimit client body: " << obj.GetLimitClientBody();
  return os;
}

std::ostream& operator<<(std::ostream& os, const LocationContext& obj) {
  os << "index: ";
  if (obj.GetIndex().size() == 0)
    os << "no set";
  else {
    for (std::vector<std::string>::const_iterator it = obj.GetIndex().begin();
         it != obj.GetIndex().end(); it++)
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
  os << "\nallow method: ";
  const std::map<std::string, bool> allow_method = obj.GetAllowMethod();
  for (std::map<std::string, bool>::const_iterator it = allow_method.begin();
       it != allow_method.end(); ++it) {
    os << (it->second ? it->first + " " : "");
  }
  os << "\ncgiPath: ";
  if (obj.GetCgiPath().size() == 0)
    os << "no set";
  else {
    for (std::vector<std::string>::const_iterator it = obj.GetCgiPath().begin();
         it != obj.GetCgiPath().end(); it++)
      os << *it << " ";
  }
  os << "\ncgiExtention: ";
  if (obj.GetCgiExtention().size() == 0)
    os << "no set";
  else {
    for (std::vector<std::string>::const_iterator it =
             obj.GetCgiExtention().begin();
         it != obj.GetCgiExtention().end(); it++)
      os << *it << " ";
  }
  os << "\nautoindex: " << (obj.GetCnaAutoIndex() == true ? "on" : "off");
  os << "\nreturn: " << (!obj.GetReturn().empty() ? obj.GetReturn() : "no set");
  os << "\nalias: " << (!obj.GetAlias().empty() ? obj.GetAlias() : "no set");
  os << "\nlimit client body: " << obj.GetLimitClientBody();
  return os;
}
