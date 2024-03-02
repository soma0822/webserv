#include "http_request_parser.hpp"

#include <iostream>

HTTPRequestParser::HTTPRequestParser() : request_(NULL), row_line_("") {}
HTTPRequestParser::~HTTPRequestParser() {}
HTTPRequestParser::HTTPRequestParser(const HTTPRequestParser &other) {
  (void)other;
}
HTTPRequestParser &HTTPRequestParser::operator=(
    const HTTPRequestParser &other) {
  (void)other;
  return *this;
}

const HTTPRequest *HTTPRequestParser::Execute(std::string request_line) {
  if (request_ == NULL) request_ = new HTTPRequest();
  // setterと形式があってるかのエラーチェック
  return request_;
  (void)request_line;
}
