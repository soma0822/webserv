#ifndef LOCATION_CONTEXT_HPP
#define LOCATION_CONTEXT_HPP

#include <iostream>
#include <vector>
#include <map>

class LocationContext{
public:
  LocationContext();
  LocationContext(const LocationContext& other);
  LocationContext &operator=(const LocationContext& other);
  //ゲッター
  bool get_can_auto_index() const;
  int get_limit_client_body() const;
  const std::string& get_return() const;
  const std::string& get_alias() const;
  const std::string& get_root() const;
  const std::vector<std::string>& get_index() const;
  const std::vector<std::string>& get_cgi_path() const;
  const std::vector<std::string>& get_cgi_extention() const;
  const std::map<std::string, bool>& get_allow_method() const;
  const std::map<std::string, std::string>& get_error_page() const;
  //セッター
  void set_can_auto_index(bool);
  void set_limit_client_body(int);
  void set_return(const std::string &);
  void set_alias(const std::string &);
  void set_root(const std::string &);
  void add_index(const std::string &);
  void add_cgi_path(const std::string &);
  void add_cgi_extention(const std::string &);
  void add_allow_method(const std::string &);
  void add_error_page(const std::string &, const std::string &);

private:
  bool can_auto_index_;
  int limit_client_body_;
  std::string return_;
  std::string alias_;
  std::string root_;
  std::vector<std::string> index_;
  std::vector<std::string> cgi_path_;
  std::vector<std::string> cgi_extention_;
  std::map<std::string, bool> allow_method_;
  std::map<std::string, std::string> error_page_;
};

std::ostream& operator<<(std::ostream& os, LocationContext& obj);
std::ostream& operator<<(std::ostream& os, const LocationContext& obj);
#endif