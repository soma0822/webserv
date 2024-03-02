#ifndef WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP
#define WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP

#include <string>

#include "http_request.hpp"

class HTTPRequestParser {
 public:
  HTTPRequestParser();
  ~HTTPRequestParser();

  const HTTPRequest *Execute(std::string request_line);

 private:
  HTTPRequest *request_;
  std::string row_line_;

  HTTPRequestParser(const HTTPRequestParser &other);
  HTTPRequestParser &operator=(const HTTPRequestParser &other);
};

#endif  // WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP
