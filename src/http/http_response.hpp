#ifndef WEBSERV_SRC_HTTP_HTTP_RESPONSE_HPP_
#define WEBSERV_SRC_HTTP_HTTP_RESPONSE_HPP_

#include <map>
#include <string>

#include "http_status_code.hpp"

class HTTPResponse {
 public:
  HTTPResponse();
  HTTPResponse(const HTTPResponse &other);
  HTTPResponse &operator=(const HTTPResponse &other);
  ~HTTPResponse();

  // アクセサー
  void SetHttpVersion(const std::string &http_version);
  const std::string &GetHttpVersion() const;
  void SetStatusCode(http::StatusCode status_code);
  http::StatusCode GetStatusCode() const;
  void AddHeader(const std::string &key, const std::string &value);
  const std::map<std::string, std::string> &GetHeaders() const;
  void SetBody(const std::string &body);
  const std::string &GetBody() const;

  std::string ToString() const;

 private:
  std::string http_version_;
  http::StatusCode status_code_;
  std::map<std::string, std::string> headers_;
  std::string body_;
};

#endif  // WEBSERV_SRC_HTTP_HTTP_RESPONSE_HPP_
