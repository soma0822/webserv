#ifndef WEBSERVE_SRC_HTTP_A_PARSER_HPP
#define WEBSERVE_SRC_HTTP_A_PARSER_HPP

#include <algorithm>
#include <iostream>
#include <string>

#include "http_request.hpp"
#include "result.hpp"
#include "string_utils.hpp"

class AParser {
 public:
  enum ReturnStatus {
    kBadRequest = 0,
    kNotEnough = 1,
    kOk = 2,
    kEndParse = 3,
  };
  AParser();
  AParser(const AParser &other);
  virtual ~AParser();

  virtual const Result<HTTPRequest *, int> Parser(std::string request_line) = 0;

 protected:
  enum StatusFlag { kBeforeProcess, kNeedHeader, kEndHeader, kNeedBody };
  enum chunked_state {
    kNeedChunkedSize,
    kNeedChunkedBody,
  };
  HTTPRequest *request_;
  std::string row_line_;
  int parser_state_;

  AParser &operator=(const AParser &other);

  bool IsFillRequestLine();
  bool IsFillHeaders();
  bool IsFillBody();
  bool IsChunked();
  bool IsContentLength();
  bool CheckNeedBodyHeader();

  int SetRequestLine();
  int SetRequestHeaders();
  int SetHeader();
  int CheckHeader();
  int SetRequestBody();
  int SetChunkedBody();
  int BadChunkedBody(int &chunked_state, size_t &chunked_size);
  int SetBody();

  const Result<HTTPRequest *, int> BadRequest();
  const Result<HTTPRequest *, int> OkRequest();

  std::string StrToUpper(std::string s);
};

#endif  // WEBSERVE_SRC_HTTP_A_PARSER_HPP
