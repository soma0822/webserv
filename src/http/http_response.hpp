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
  void SetStatusCode(http::StatusCode status_code);
  http::StatusCode GetStatusCode() const;
  void SetHeader(const std::string &key, const std::string &value);
  std::string GetHeader(const std::string &key);
  void SetBody(const std::string &body);
  const std::string &GetBody() const;

  std::string ToString() const;

 private:
  http::StatusCode status_code_;
  std::map<std::string, std::string> headers_;
  std::string body_;
};

#endif  // WEBSERV_SRC_HTTP_HTTP_RESPONSE_HPP_
