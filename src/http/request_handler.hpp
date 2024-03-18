#ifndef WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_
#define WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_

#include "config.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

class RequestHandler {
 public:
  static HTTPResponse *Handle(const IConfig &config, const HTTPRequest *request,
                              const std::string &port, const std::string &ip);
  static HTTPResponse *Get(const IServerContext &server_ctx,
                           const HTTPRequest *request);

 private:
  RequestHandler();
  RequestHandler(const RequestHandler &other);
  RequestHandler &operator=(const RequestHandler &other);
  ~RequestHandler();

  static std::string ResolvePath(const IServerContext &server_ctx,
                                 const std::string &uri);
};

#endif  // WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_
