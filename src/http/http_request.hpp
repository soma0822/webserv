#ifndef WEBSERV_SRC_HTTP_HTTP_REQUEST_HPP_
#define WEBSERV_SRC_HTTP_HTTP_REQUEST_HPP_

#include "http_response.hpp"

class AHTTPRequest {
 public:
  virtual HTTPResponse Execute() = 0;
};

#endif  // WEBSERV_SRC_HTTP_HTTP_REQUEST_HPP_
