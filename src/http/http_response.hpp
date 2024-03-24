#ifndef WEBSERV_SRC_HTTP_HTTP_RESPONSE_HPP_
#define WEBSERV_SRC_HTTP_HTTP_RESPONSE_HPP_

#include <map>
#include <string>

#include "config.hpp"
#include "http_status_code.hpp"
#include "option.hpp"

class HTTPResponse {
 public:
  class Builder {
   public:
    Builder();
    ~Builder();
    Builder &SetStatusCode(http::StatusCode status_code);
    Builder &AddHeader(const std::string &key, const std::string &value);
    Builder &SetBody(const std::string &body);
    Option<HTTPResponse *> Build();

   private:
    static const std::string kHTTPVersion;
    HTTPResponse *response_;

    Builder(const Builder &other);
    const Builder &operator=(const Builder &other);
  };

  HTTPResponse();
  HTTPResponse(const HTTPResponse &other);
  HTTPResponse &operator=(const HTTPResponse &other);
  ~HTTPResponse();

  // アクセサー
  void SetHTTPVersion(const std::string &http_version);
  const std::string &GetHTTPVersion() const;
  void SetStatusCode(http::StatusCode status_code);
  http::StatusCode GetStatusCode() const;
  void AddHeader(const std::string &key, const std::string &value);
  const std::map<std::string, std::string> &GetHeaders() const;
  void SetBody(const std::string &body);
  const std::string &GetBody() const;

  std::string ToString();

 private:
  std::string http_version_;
  http::StatusCode status_code_;
  std::map<std::string, std::string> headers_;
  std::string body_;
};

Option<HTTPResponse *> GenerateErrorResponse(http::StatusCode status_code,
                                             const IConfig &config);

#endif  // WEBSERV_SRC_HTTP_HTTP_RESPONSE_HPP_
