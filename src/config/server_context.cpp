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
}

//ゲッター
const std::string &ServerContext::get_ip() const{
  return ip_;
}
const std::vector<std::string> &ServerContext::get_port() const{
  return port_;
}
const std::vector<std::string> &ServerContext::get_server_name() const{
  return server_name_;
}
const std::map<std::string, LocationContext> &ServerContext::get_location() const{
  return location_;
}

//セッター
void ServerContext::set_ip(const std::string & ip){
  if (ip_ == "")
    ip_ = ip;
}
void ServerContext::add_port(const std::string & port){
  port_.push_back(port);
}
void ServerContext::add_server_name(const std::string & server_name){
  server_name_.push_back(server_name);
}
void ServerContext::add_location(const std::string &key, const LocationContext &value){
  std::map<std::string, LocationContext>::iterator it = location_.find(key);
  if (it == location_.end())
    location_[key] = value;
}