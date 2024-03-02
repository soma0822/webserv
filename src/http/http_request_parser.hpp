#ifndef WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP
#define WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP

#include <string>

#include "http_request.hpp"

class HTTPRequestParser {
 public:
  static const HTTPRequest *GetRequestInstance(std::string request_line,
                                         std::string port);

 private:
  HTTPRequestParser();
  ~HTTPRequestParser();
  HTTPRequestParser(const HTTPRequestParser &other);
  HTTPRequestParser &operator=(const HTTPRequestParser &other);
};

#endif  // WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP
