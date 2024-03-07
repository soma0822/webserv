#ifndef WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP
#define WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP

#include <algorithm>
#include <iostream>
#include <string>

#include "http_request.hpp"
#include "result.hpp"
#include "string_utils.hpp"

class HTTPRequestParser {
 public:
  enum ReturnStatus {
    kBadRequest = 0,
    kNotEnough = 1,
    kOk = 2,
  };
  HTTPRequestParser();
  ~HTTPRequestParser();

  const Result<HTTPRequest *, int> Parser(std::string request_line);

 private:
  enum StatusFlag { kBeforeProcess, kNeedHeader, kEndHeader, kNeedBody };
  HTTPRequest *request_;
  std::string row_line_;
  int parser_state_;

  HTTPRequestParser(const HTTPRequestParser &other);
  HTTPRequestParser &operator=(const HTTPRequestParser &other);

  bool IsFillRequestLine();
  bool IsFillHeaders();
  bool IsFillBody();
  bool IsNeedBody();

  int SetRequestLine();
  int SetRequestHeaders();
  int SetRequestBody();
  int SetChunkedBody();

  const Result<HTTPRequest *, int> BadRequest();
  const Result<HTTPRequest *, int> OkRequest();

  std::string StrToUpper(std::string s);
};

#endif  // WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP
