#ifndef WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_
#define WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_

#include "http_request.hpp"
#include "http_response.hpp"

class RequestHandler {
 public:
  static HTTPResponse *Handle(const HTTPRequest *request);

 private:
  RequestHandler();
  RequestHandler(const RequestHandler &other);
  RequestHandler &operator=(const RequestHandler &other);
  ~RequestHandler();
};

#endif  // WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_
