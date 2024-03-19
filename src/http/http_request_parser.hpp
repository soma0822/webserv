#ifndef WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP
#define WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP

#include "a_parser.hpp"
class AParser;

class HTTPRequestParser : public AParser {
 public:
  HTTPRequestParser();
  ~HTTPRequestParser();

  const Result<HTTPRequest *, int> Parser(std::string request_line);
  int SetHeader();
};

#endif  // WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP
