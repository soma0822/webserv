#include "http_request_parser.hpp"

// canonical
HTTPRequestParser::HTTPRequestParser() {}
HTTPRequestParser::~HTTPRequestParser() { delete request_; }

// リクエストのパース
const Result<HTTPRequest *, int> HTTPRequestParser::Parser(
    std::string request_line) {
  int return_state;
  row_line_ = row_line_ + request_line;
  if (row_line_ == "") return Err(kEndParse);
  if (request_ == NULL) request_ = new HTTPRequest();
  // requestlineの内容を確認
  if (parser_state_ == kBeforeProcess) {
    if (SetRequestLine() == kBadRequest) {
      return BadRequest();
    }
    parser_state_ = kNeedHeader;
  }
  // Headerの内容を確認
  if (parser_state_ == kNeedHeader) {
    return_state = SetRequestHeaders();
    if (return_state == kBadRequest)
      return BadRequest();
    else if (return_state == kNotEnough)
      return Err(kNotEnough);
    else {
      if (CheckNeedBodyHeader() == false) return BadRequest();
    }
  }
  // bodyの内容を確認
  if (parser_state_ == kNeedBody) {
    return_state = SetRequestBody();
    if (return_state == kNotEnough)
      return Err(kNotEnough);
    else if (return_state == kBadRequest)
      return BadRequest();
  }
  return OkRequest();
}
