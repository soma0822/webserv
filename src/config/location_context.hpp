#ifndef WEBSERV_SRC_CONFIG_LOCATION_CONTEXT_HPP
#define WEBSERV_SRC_CONFIG_LOCATION_CONTEXT_HPP

#include <algorithm>
#include <climits>
#include <iostream>
#include <map>
#include <vector>

class LocationContext {
 public:
  LocationContext();
  LocationContext(const LocationContext &other);
  LocationContext &operator=(const LocationContext &other);
  // ゲッター
  bool GetCnaAutoIndex() const;
  int GetLimitClientBody() const;
  const std::string &GetPath() const;
  const std::string &GetReturn() const;
  const std::string &GetAlias() const;
  const std::string &GetRoot() const;
  const std::string &GetIndex() const;
  const std::vector<std::string> &GetCgiPath() const;
  const std::vector<std::string> &GetCgiExtention() const;
  const std::map<std::string, bool> &GetAllowMethod() const;
  const std::map<std::string, std::string> &GetErrorPage() const;
  // セッター
  void SetCanAutoIndex(bool can_auto_inde);
  void SetLimitClientBody(int limit_client_body_bytes);
  void SetPath(const std::string &path);
  void SetReturn(const std::string &ret);
  void SetAlias(const std::string &alias);
  void SetRoot(const std::string &root);
  void SetIndex(const std::string &index);
  void AddCgiPath(const std::string &cgi_path);
  void AddCgiExtention(const std::string &cgi_extention);
  void AddAllowMethod(const std::string &key);
  void AddErrorPage(const std::string &key, const std::string &value);

 private:
  bool can_auto_index_;
  int limit_client_body_bytes_;
  std::string path_;
  std::string return_;
  std::string alias_;
  std::string root_;
  std::string index_;
  std::vector<std::string> cgi_path_;
  std::vector<std::string> cgi_extention_;
  std::map<std::string, bool> allow_method_;
  std::map<std::string, std::string> error_page_;
};

std::ostream &operator<<(std::ostream &os, LocationContext &obj);
std::ostream &operator<<(std::ostream &os, const LocationContext &obj);
#endif
