#ifndef WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_
#define WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_

#include "config.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

class RequestHandler {
 public:
  static HTTPResponse *Handle(const IConfig &config, RequestContext req_ctx);
  static HTTPResponse *Get(const IConfig &config, RequestContext req_ctx);
  static HTTPResponse *Post(const IConfig &config, RequestContext req_ctx);
  static HTTPResponse *Delete(const IConfig &config, RequestContext req_ctx);

 private:
  RequestHandler();
  RequestHandler(const RequestHandler &other);
  RequestHandler &operator=(const RequestHandler &other);
  ~RequestHandler();

  static const int kMaxUriLength = 2048;

  static HTTPResponse *GenerateAutoIndexPage(const IConfig &config,
                                             const HTTPRequest *request,
                                             const std::string &abs_path);
};

#endif  // WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_
