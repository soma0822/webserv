#ifndef WEBSERVE_SRC_HTTP_A_PARSER_HPP
#define WEBSERVE_SRC_HTTP_A_PARSER_HPP

#include <algorithm>
#include <iostream>
#include <string>

#include "http_request.hpp"
#include "http_status_code.hpp"
#include "result.hpp"
#include "string_utils.hpp"
#include "validation.hpp"

class AParser {
 public:
  enum ReturnStatus {
    kOk = 0,
    kNotEnough = 1,
    kEndParse = 2,
    kBadRequest = http::kBadRequest,
    kHttpVersionNotSupported = http::kHttpVersionNotSupported,
    kPayloadTooLarge = http::kPayloadTooLarge
  };
  AParser();
  AParser(const AParser &other);
  virtual ~AParser();

  virtual const Result<HTTPRequest *, int> Parser(std::string request_line) = 0;

 protected:
  enum StatusFlag { kBeforeProcess, kNeedHeader, kEndHeader, kNeedBody };
  enum ChunkedStateFlag {
    kNeedChunkedSize,
    kNeedChunkedBody,
    kMaxBodySize = 100000000
  };
  struct ChunkedState {
    int chunked_state;
    size_t chunked_size;
    size_t total_size;
  };
  HTTPRequest *request_;
  std::string row_line_;
  int parser_state_;

  AParser &operator=(const AParser &other);

  std::pair<std::string, int> ParsePart(std::string &str,
                                        const std::string &delimiter,
                                        int errorcode);
  int CheckProtocol();

  bool IsFillRequestLine();
  bool IsFillHeaders();
  bool IsFillBody();
  bool IsChunked();
  bool IsContentLength();
  bool CheckNeedBodyHeader();

  int SetRequestLine();
  int SetRequestHeaders();
  virtual int SetHeader();
  int CheckHeader();
  int SetRequestBody();
  int SetChunkedBody();
  int ResetChunkedBody(ChunkedState &state, int status_code);
  int SetBody();

  const Result<HTTPRequest *, int> OkRequest();
  const Result<HTTPRequest *, int> ErrRequest(int status_code);
};

#endif  // WEBSERVE_SRC_HTTP_A_PARSER_HPP
