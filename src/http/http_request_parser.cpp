#include "http_request_parser.hpp"

#include <unistd.h>

#include "a_parser.hpp"

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
    return_state = SetRequestLine();
    if (return_state == kBadRequest)
      return BadRequest();
    else if (return_state == kNotEnough)
      return Err(kNotEnough);
    else if (return_state == kHttpVersionNotSupported)
      return HttpVersionNotSupported();
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

int HTTPRequestParser::SetHeader() {
  std::string request_line;
  std::string headline, key, value;
  size_t pos = 0, key_pos = 0;

  while (1) {
    pos = row_line_.find("\r\n");
    if (pos == std::string::npos) return kNotEnough;  // 改行がない時
    request_line = row_line_.substr(0, pos);
    row_line_ = row_line_.substr(pos + 2);
    if (request_line == "") break;  // Headerの最終の空行の処理
    key_pos = request_line.find(":");
    if (key_pos == std::string::npos || key_pos == 0)
      return kBadRequest;  // :がない、もしくは左辺が何もない
    key = request_line.substr(0, key_pos);
    value = request_line.substr(key_pos + 1);
    if (string_utils::StrToUpper(key) == "HOST" &&
        request_->GetHeaders().count("HOST") > 0)
      return kBadRequest;  // Hostが複数ある時
    request_->AddHeader(string_utils::StrToUpper(key),
                        string_utils::SkipSpace(value));
  }
  return kOk;
}
