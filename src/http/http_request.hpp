#ifndef WEBSERV_SRC_HTTP_HTTP_REQUEST_HPP
#define WEBSERV_SRC_HTTP_HTTP_REQUEST_HPP

#include <arpa/inet.h>

#include <map>
#include <string>

class HTTPRequest {
 public:
  HTTPRequest();
  HTTPRequest(const HTTPRequest &other);
  HTTPRequest &operator=(const HTTPRequest &other);
  ~HTTPRequest();

  void SetMethod(const std::string &method);
  void SetUri(const std::string &uri);
  void SetProtocol(const std::string &protocol);
  void SetVersion(const std::string &version);
  void SetHostHeader(const std::string &host_header);
  void AddHeader(const std::string &key, const std::string &value);
  void AddBody(const std::string &body);
  const std::string &GetMethod() const;
  const std::string &GetUri() const;
  const std::string &GetProtocol() const;
  const std::string &GetVersion() const;
  const std::string &GetHostHeader() const;
  const std::map<std::string, std::string> &GetHeaders() const;
  const std::string &GetBody() const;

 private:
  std::string method_;
  std::string uri_;
  std::string protocol_;
  std::string version_;
  std::string host_header_;
  std::map<std::string, std::string> headers_;
  std::string body_;
};

struct RequestContext {
  HTTPRequest *request;
  std::string port;
  std::string ip;
  struct sockaddr_in client_addr;
  int fd;
  int count;
};

#endif  // WEBSERV_SRC_HTTP_HTTP_REQUEST_HPP
