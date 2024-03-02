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
}

const HTTPRequest *HTTPRequestParser::GetRequestInstance(
    std::string request_line) {
  if (request_ == NULL) HTTPRequest *request = new HTTPRequest();
  // setterと形式があってるかのエラーチェック
  return request_;
}
