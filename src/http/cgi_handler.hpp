#ifndef WEBSERV_SRC_HTTP_CGI_HANDLER_HPP_
#define WEBSERV_SRC_HTTP_CGI_HANDLER_HPP_

#include "http_response.hpp"
#include "result.hpp"
#include "request_handler.hpp"
#include "http_request.hpp"
#include "option.hpp"

class CGIHandler {
 public:
  static Option<HTTPResponse *> Handle(const IConfig &config, HTTPRequest *cgi_req, RequestContext req_ctx);

  private:
  CGIHandler();
  CGIHandler(const CGIHandler &other);
  CGIHandler &operator=(const CGIHandler &other);
  ~CGIHandler();
};
enum ReturnStatus {
  ExecuteCGI
};

#endif
