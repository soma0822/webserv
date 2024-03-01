#ifndef WEBSERV_SRC_HTTP_HTTP_REQUEST_HPP
#define WEBSERV_SRC_HTTP_HTTP_REQUEST_HPP

#include <map>
#include <string>

class HttpRequest {
 public:
  HttpRequest();
  ~HttpRequest();
  void SetMethod(const std::string &method);
  void SetUri(const std::string &uri);
  void SetProtocol(const std::string &protocol);
  void SetVersion(const std::string &version);
  void SetHostHeader(const std::string &host_header);
  void SetHeaders(const std::map<std::string, std::string> &headers);
  void SetBody(const std::string &body);
  std::string GetMethod() const;
  std::string GetUri() const;
  std::string GetProtocol() const;
  std::string GetVersion() const;
  std::string GetHostHeader() const;
  std::map<std::string, std::string> GetHeaders() const;
  std::string GetBody() const;

 private:
  std::string method_;
  std::string uri_;
  std::string protocol_;
  std::string version_;
  std::string host_header_;
  std::map<std::string, std::string> headers_;
  std::string body_;
};

#endif  // WEBSERV_SRC_HTTP_HTTP_REQUEST_HPP
