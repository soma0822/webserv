#ifndef WEBSERV_SRC_CONFIG_LOCATION_CONTEXT_HPP
#define WEBSERV_SRC_CONFIG_LOCATION_CONTEXT_HPP

#include <climits>
#include <iostream>
#include <map>
#include <vector>

class LocationContext {
 public:
  LocationContext();
  LocationContext(const LocationContext& other);
  LocationContext& operator=(const LocationContext& other);
  // ゲッター
  bool GetCnaAutoIndex() const;
  int GetLimitClientBody() const;
  const std::string& GetReturn() const;
  const std::string& GetAlias() const;
  const std::string& GetRoot() const;
  const std::vector<std::string>& GetIndex() const;
  const std::vector<std::string>& GetCgiPath() const;
  const std::vector<std::string>& GetCgiExtention() const;
  const std::map<std::string, bool>& GetAllowMethod() const;
  const std::map<std::string, std::string>& GetErrorPage() const;
  // セッター
  void SetCanAutoIndex(bool);
  void SetLimitClientBody(int);
  void SetReturn(const std::string&);
  void SetAlias(const std::string&);
  void SetRoot(const std::string&);
  void AddIndex(const std::string&);
  void AddCgiPath(const std::string&);
  void AddCgiExtention(const std::string&);
  void AddAllowMethod(const std::string&);
  void AddErrorPage(const std::string&, const std::string&);

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
