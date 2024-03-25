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
  bool GetCanAutoIndex() const;
  int GetLimitClientBody() const;
  const std::string &GetPath() const;
  const std::string &GetReturn() const;
  const std::string &GetAlias() const;
  const std::string &GetRoot() const;
  const std::string &GetIndex() const;
  const std::vector<std::string> &GetCgiExtension() const;
  const std::map<std::string, bool> &GetAllowMethod() const;
  bool IsAllowedMethod(const std::string &key);
  // セッター
  void SetCanAutoIndex(bool can_auto_inde);
  void SetLimitClientBody(int limit_client_body_bytes);
  void SetPath(const std::string &path);
  void SetReturn(const std::string &ret);
  void SetAlias(const std::string &alias);
  void SetRoot(const std::string &root);
  void SetIndex(const std::string &index);
  void AddCgiExtension(const std::string &cgi_extension);
  void AddAllowMethod(const std::string &key);

 private:
  bool can_auto_index_;
  int limit_client_body_bytes_;
  std::string path_;
  std::string return_;
  std::string alias_;
  std::string root_;
  std::string index_;
  std::vector<std::string> cgi_extension_;
  std::map<std::string, bool> allow_method_;
};

std::ostream &operator<<(std::ostream &os, LocationContext &obj);
std::ostream &operator<<(std::ostream &os, const LocationContext &obj);
#endif
