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

int HTTPRequestParser::SetHeader() {
  std::string request_line = row_line_;
  std::string headline, key, value;
  size_t pos = 0;
  size_t key_pos = 0;
  size_t value_pos = 0;

  while (1) {
    key_pos = request_line.find(":");
    if (key_pos == std::string::npos) break;  // 最後まで見てしまった時
    pos = request_line.find("\r\n");
    if (pos == std::string::npos) break;    // 改行がない時
    if (pos < key_pos) return kBadRequest;  // 改行が先にある時
    if (key_pos == 0) return kBadRequest;   // 左辺が何もない
    key = request_line.substr(0, key_pos);
    if (pos == key_pos + 1) {
      value_pos = 0;
      value = "";
    } else {
      value_pos = pos - key_pos - 1;
      value =
          string_utils::SkipSpace(request_line.substr(key_pos + 1, value_pos));
    }
    if (StrToUpper(key) == "HOST" && request_->GetHeaders().count("HOST") > 0)
      return kBadRequest;
    request_line = request_line.substr(key_pos + value_pos + 3);
    request_->AddHeader(StrToUpper(key), value);
  }
  // headerの終わりの確認
  row_line_ = request_line;
  // まだheaderが続いている場合
  if (request_line.find("\r\n") != 0) return kNotEnough;
  row_line_ = request_line.substr(2);
  return kOk;
}
