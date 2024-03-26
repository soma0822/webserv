#include "cgi_parser.hpp"

// canonical
CGIParser::CGIParser() {}
CGIParser::~CGIParser() { delete request_; }

// リクエストのパース
const Result<HTTPRequest *, int> CGIParser::Parser(std::string request_line) {
  row_line_ = row_line_ + request_line;
  if (row_line_ == "") return Err(kEndParse);
  if (request_ == NULL) request_ = new HTTPRequest();
  // Headerの内容を確認
  SetHeader();
  // bodyをセット
  SetBody();
  return OkRequest();
}
