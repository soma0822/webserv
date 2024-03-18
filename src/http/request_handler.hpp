#ifndef WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_
#define WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_

#include "config.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

class RequestHandler {
 public:
  static HTTPResponse *Handle(const IConfig &config, const HTTPRequest *request,
                              const std::string &port, const std::string &ip);
  static HTTPResponse *Get(const IConfig &config, const HTTPRequest *request,
                           const std::string &port, const std::string &ip);
  static HTTPResponse *Post(const IConfig &config, const HTTPRequest *request,
                            const std::string &port, const std::string &ip);

 private:
  RequestHandler();
  RequestHandler(const RequestHandler &other);
  RequestHandler &operator=(const RequestHandler &other);
  ~RequestHandler();
};

#endif  // WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_
