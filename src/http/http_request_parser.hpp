#ifndef WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP
#define WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP

#include <algorithm>
#include <iostream>
#include <string>

#include "a_parser.hpp"
#include "http_request.hpp"
#include "result.hpp"
#include "string_utils.hpp"

class AParser;

class HTTPRequestParser : public AParser {
 public:
  HTTPRequestParser();
  ~HTTPRequestParser();

  const Result<HTTPRequest *, int> Parser(std::string request_line);
};

#endif  // WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP
