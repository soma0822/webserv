#ifndef WEBSERV_SRC_HTTP_CGI_HANDLER_HPP_
#define WEBSERV_SRC_HTTP_CGI_HANDLER_HPP_

#include "http_request.hpp"
#include "http_response.hpp"
#include "option.hpp"
#include "request_handler.hpp"
#include "result.hpp"

class CGIHandler {
 public:
  static Option<HTTPResponse *> Handle(const IConfig &config,
                                       HTTPRequest *cgi_req,
                                       RequestContext req_ctx);

 private:
  CGIHandler();
  CGIHandler(const CGIHandler &other);
  CGIHandler &operator=(const CGIHandler &other);
  ~CGIHandler();
};
enum ReturnStatus { ExecuteCGI };

#endif
