#ifndef WEBSERVE_SRC_HTTP_CGI_PARSER_HPP
#define WEBSERVE_SRC_HTTP_CGI_PARSER_HPP

#include "a_parser.hpp"

class AParser;

class CGIParser : public AParser {
 public:
  CGIParser();
  ~CGIParser();

  const Result<HTTPRequest *, int> Parser(std::string request_line);
};

#endif  // WEBSERVE_SRC_HTTP_CGI_PARSER_HPP
