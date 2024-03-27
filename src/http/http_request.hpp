#ifndef WEBSERV_SRC_HTTP_HTTP_REQUEST_HPP
#define WEBSERV_SRC_HTTP_HTTP_REQUEST_HPP

#include <arpa/inet.h>

#include <map>
#include <string>
#include <iostream>

class HTTPRequest {
 public:
  HTTPRequest();
  HTTPRequest(const HTTPRequest &other);
  HTTPRequest &operator=(const HTTPRequest &other);
  ~HTTPRequest();

  void SetMethod(const std::string &method);
  void SetUri(const std::string &uri);
  void SetQuery(const std::string &query);
  void SetProtocol(const std::string &protocol);
  void SetVersion(const std::string &version);
  void SetHostHeader(const std::string &host_header);
  void AddHeader(const std::string &key, const std::string &value);
  void AddBody(const std::string &body);
  const std::string &GetMethod() const;
  const std::string &GetUri() const;
  const std::string &GetQuery() const;
  const std::string &GetProtocol() const;
  const std::string &GetVersion() const;
  const std::string &GetHostHeader() const;
  const std::map<std::string, std::string> &GetHeaders() const;
  const std::string &GetBody() const;

 private:
  std::string method_;
  std::string uri_;
  std::string query_;
  std::string protocol_;
  std::string version_;
  std::string host_header_;
  std::map<std::string, std::string> headers_;
  std::string body_;
};

std::ostream &operator<<(std::ostream &os, HTTPRequest &obj);
std::ostream &operator<<(std::ostream &os, const HTTPRequest &obj);

struct RequestContext {
  HTTPRequest *request;
  std::string port;
  std::string ip;
  struct sockaddr_in client_addr;
  int fd;
  int count;
};

#endif  // WEBSERV_SRC_HTTP_HTTP_REQUEST_HPP
