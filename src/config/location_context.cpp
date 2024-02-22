#include "location_context.hpp"

LocationContext::LocationContext(): can_auto_index_(false), limit_client_body_(INT_MAX), return_(""), alias_(""), root_(""){
  allow_method_["GET"] = false;
  allow_method_["POST"] = false;
  allow_method_["DELETE"] = false;
}
LocationContext::LocationContext(const LocationContext& other){
  *this = other;
}
LocationContext &LocationContext::operator=(const LocationContext& other){
  if (this == &other){
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
//ゲッター
bool LocationContext::get_can_auto_index() const{
  return can_auto_index_;
}
int LocationContext::get_limit_client_body() const{
  return limit_client_body_;
}
const std::string& LocationContext::get_return() const{
  return return_;
}
const std::string& LocationContext::get_alias() const{
  return alias_;
}
const std::string& LocationContext::get_root() const{
  return root_;
}
const std::vector<std::string>& LocationContext::get_index() const{
  return index_;
}
const std::vector<std::string>& LocationContext::get_cgi_path() const{
  return cgi_path_;
}
const std::vector<std::string>& LocationContext::get_cgi_extention() const{
  return cgi_extention_;
}
const std::map<std::string, bool>& LocationContext::get_allow_method() const{
  return allow_method_;
}
const std::map<std::string, std::string>& LocationContext::get_error_page() const{
  return error_page_;
}
//セッター
void LocationContext::set_can_auto_index(bool can_auto_index){
  can_auto_index_ = can_auto_index;
}
void LocationContext::set_limit_client_body(int limit_client_body){
  limit_client_body_ = limit_client_body;
}
void LocationContext::set_return(const std::string &ret){
  return_ = ret;
}
void LocationContext::set_alias(const std::string &alias){
  alias_ = alias;
}
void LocationContext::set_root(const std::string &root){
  root_ = root;
}
void LocationContext::add_index(const std::string &index){
  index_.push_back(index);
}
void LocationContext::add_cgi_path(const std::string &cgi_path){
  cgi_path_.push_back(cgi_path);
}
void LocationContext::add_cgi_extention(const std::string &cgi_extention){
  cgi_extention_.push_back(cgi_extention);
}
void LocationContext::add_allow_method(const std::string &key){
  allow_method_[key] = true;
}
void LocationContext::add_errorPage(const std::string &key, const std::string &value){
  std::map<std::string, std::string>::iterator it = error_page_.find(key);
  if (it == error_page_.end())
    error_page_[key] = value;
}

// 出力
std::ostream& operator<<(std::ostream& os, LocationContext& obj){
	os << "index: ";
  for (std::vector<std::string>::const_iterator it = obj.get_index().begin(); it != obj.get_index().end(); it++){
    os << *it << " ";
  }
	os << "\nerror page: ";
	for (std::map<std::string, std::string>::const_iterator it = obj.get_error_page().begin(); it != obj.get_error_page().end(); it++){
		os << it->first << "[" << it->second << "]" << "    ";
	}
	os << "\nroot: "<< obj.get_root();
	const std::map<std::string, bool> allow_method = obj.get_allow_method();
	for (std::map<std::string, bool>::const_iterator it = allow_method.begin(); it != allow_method.end(); ++it) {
		os << (it->second ? it->first + " " : "");
	}
	os << "\ncgiPath:";
	for (std::vector<std::string>::const_iterator it = obj.get_cgi_path
	}
	os << "\ncgiExtention:";
	for (unsigned long long i = 0; i < obj.getCgiExtention().size(); i++){
		os << " ";
		os << obj.getCgiExtention()[i];
	}
	os << "\nautoindex: " << (obj.getCanAutoIndex() == true ? "on" : "off");
	os << "\nreturn: " << (!obj.getReturn().empty() ? obj.getReturn() : "no set");
	os << "\nalias: " << (!obj.getAlias().empty() ? obj.getAlias() : "no set");
	os << "\nlimit client body: " << obj.getLimitClientBody();
	return os;
}