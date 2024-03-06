#ifndef WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP
#define WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP

#include <algorithm>
#include <iostream>
#include <string>

#include "http_request.hpp"
#include "result.hpp"
#include "string_utils.hpp"

#define kBadRequest 0
#define kNotEnough 1
#define kOk 2

class HTTPRequestParser {
 public:
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

  const Result<HTTPRequest *, int> BadRequest();
  const Result<HTTPRequest *, int> OkRequest();

  std::string StrToUpper(std::string s);
  std::string SkipSpace(std::string s);

  // データのセットを関数ポインタとテンプレートでやりたいけど、よく分からん。
  //   template <typename F>
  //   int SetData(std::string search_str,  F *foo, std::string &request_line) {
  //     try {
  //       size_t pos = request_line.find(search_str);
  //       const std::string set_str = request_line.substr(0, pos);
  //       foo(set_str);
  //       request_line = request_line.substr(pos + 1);
  //       return kOk;
  //     } catch (std::exception &e) {
  //       return kBadRequest;
  //     }
  //   }
};

#endif  // WEBSERVE_SRC_HTTP_HTTP_REQUEST_PARSER_HPP
