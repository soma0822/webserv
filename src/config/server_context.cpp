#include "server_context.hpp"

ServerContext::ServerContext():ip_(""){}

ServerContext::ServerContext(const ServerContext &other):ip_(other.ip_){
  *this = other;
}

ServerContext &ServerContext::operator=(const ServerContext &other){
  if (this == &other){
    return *this;
  }
  ip_ = other.ip_;
  server_name_ = other.server_name_;
  location_ = other.location_;
  return *this;
}

//ゲッター
const std::string &ServerContext::get_ip() const{
  return ip_;
}
const std::string &ServerContext::get_root() const{
  return root_;
}
const std::vector<std::string> &ServerContext::get_index() const{
  return index_;
}
const std::vector<std::string> &ServerContext::get_port() const{
  return port_;
}
const std::vector<std::string> &ServerContext::get_server_name() const{
  return server_name_;
}
const std::map<std::string, std::string> &ServerContext::get_error_page() const{
  return error_page_;
}
const std::map<std::string, LocationContext> &ServerContext::get_location() const{
  return location_;
}

//セッター
void ServerContext::set_ip(const std::string & ip){
  if (ip_ == "")
    ip_ = ip;
}
void ServerContext::set_root(const std::string &root){
  root_ = root;
}
void ServerContext::add_index(const std::string &index){
  index_.push_back(index);
}
void ServerContext::add_port(const std::string & port){
  port_.push_back(port);
}
void ServerContext::add_server_name(const std::string & server_name){
  server_name_.push_back(server_name);
}
void ServerContext::add_error_page(const std::string &key, const std::string &value){
  std::map<std::string, std::string>::iterator it = error_page_.find(key);
  if (it == error_page_.end())
    error_page_[key] = value;
}
void ServerContext::add_location(const std::string &key, const LocationContext &value){
  std::map<std::string, LocationContext>::iterator it = location_.find(key);
  if (it == location_.end())
    location_[key] = value;
}

std::ostream& operator<<(std::ostream& os, ServerContext& obj){
  os << SERVER;
  std::vector<std::string> tmp = obj.get_port();
  os << "port: ";
  for (std::vector<std::string>::const_iterator it = tmp.begin(); it != tmp.end(); it++){
    os << *it << " ";
  }
  tmp = obj.get_server_name();
  os << "\nserver_name: ";
  for (std::vector<std::string>::const_iterator it = tmp.begin(); it != tmp.end(); it++){
    os << *it << " ";
  }
  tmp = obj.get_index();
  os << "\nindex: ";
  for (std::vector<std::string>::const_iterator it = tmp.begin(); it != tmp.end(); it++){
    os << *it << " ";
  }
	os << "\nerror page: ";
	for (std::map<std::string, std::string>::const_iterator it = obj.get_error_page().begin(); it != obj.get_error_page().end(); it++){
		os << it->first << "[" << it->second << "]" << "    ";
	}
	os << "\nroot: "<< obj.get_root();
	// os << "\nport: " << obj.getPort();
	os << "\nip: " << obj.get_ip();
	for (std::map<std::string, LocationContext>::const_iterator it = obj.get_location().begin(); it != obj.get_location().end(); it++){
		os << LOCATION << "\nLocation: ";
		os << it->first << "\n";
		os << it->second << "\n";
    os << "}\n";
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const ServerContext& obj){
  os << SERVER;
  std::vector<std::string> tmp = obj.get_port();
  os << "port: ";
  for (std::vector<std::string>::const_iterator it = tmp.begin(); it != tmp.end(); it++){
    os << *it << " ";
  }
  tmp = obj.get_server_name();
  os << "\nserver_name: ";
  for (std::vector<std::string>::const_iterator it = tmp.begin(); it != tmp.end(); it++){
    os << *it << " ";
  }
  tmp = obj.get_index();
  os << "\nindex: ";
  for (std::vector<std::string>::const_iterator it = tmp.begin(); it != tmp.end(); it++){
    os << *it << " ";
  }
	os << "\nerror page: ";
	for (std::map<std::string, std::string>::const_iterator it = obj.get_error_page().begin(); it != obj.get_error_page().end(); it++){
		os << it->first << "[" << it->second << "]" << "    ";
	}
	os << "\nroot: "<< obj.get_root();
	// os << "\nport: " << obj.getPort();
	os << "\nip: " << obj.get_ip();
	for (std::map<std::string, LocationContext>::const_iterator it = obj.get_location().begin(); it != obj.get_location().end(); it++){
		os << LOCATION << "\nLocation: ";
		os << it->first;
    os << " {\n";
		os << it->second << "\n";
    os << "}\n";
	}
	return os;
}
