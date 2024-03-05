#include "http_request_parser.hpp"

#include <iostream>
#include <string>

#include "http_request.hpp"

// utils
std::string HTTPRequestParser::str_toupper(std::string s) {
  for (size_t i = 0; i < s.length(); i++) {
    s[i] = std::toupper(s[i]);
  }
  return s;
}

// canonical
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

// テスト用
void PrintAll(HTTPRequest *request) {
  std::cout << "method: " << request->GetMethod() << std::endl;
  std::cout << "uri: " << request->GetUri() << std::endl;
  std::cout << "protocol: " << request->GetProtocol() << std::endl;
  std::cout << "version: " << request->GetVersion() << std::endl;
  std::cout << "host: " << request->GetHostHeader() << std::endl;
  std::cout << "body: " << request->GetBody() << std::endl;
}

// リクエストのパース
const Result<HTTPRequest *, int> HTTPRequestParser::Parser(
    std::string request_line) {
  row_line_ = request_line;
  std::cout << "row_line_: " << row_line_ << std::endl;
  if (request_ == NULL) request_ = new HTTPRequest();
  // newのエラー処理
  // requestlineの内容が全て埋まっているか確認
  std::cout << "IsFillRequestLine: " << IsFillRequestLine() << std::endl;
  if (IsFillRequestLine() == false) {
    if (SetRequestLine() == kBadRequest) {
      return Err(kBadRequest);
    }
  }
  // std::cout << "IsFillHeaders: " << IsFillHeaders() << std::endl;
  // if (IsFillHeaders() == false) {
  //   if (SetRequestHeaders() == kBadRequest) {
  //     return Err(kBadRequest);
  //   }
  // }
  // std::cout << "IsFillBody: " << IsFillBody() << std::endl;
  // if (IsFillBody() == false && IsNeedBody() == true) {
  //   if (SetRequestBody() == kBodyNotEnough) {
  //     return Err(kBodyNotEnough);
  //   }
  // }
  // // setterと形式があってるかのエラーチェック
  std::cout << "PrintAll  : " << std::endl;
  PrintAll(request_);
  std::cout << "row_line_ : " << row_line_ << std::endl;
  return Ok(request_);
  (void)request_line;
}

// 情報がぜんぶ埋まっているか確認
bool HTTPRequestParser::IsFillRequestLine() {
  if (request_->GetMethod().empty() || request_->GetUri().empty() ||
      request_->GetProtocol().empty() || request_->GetVersion().empty()) {
    return false;
  }
  return true;
}
// bool HTTPRequestParser::IsFillHeaders() {
//   if (request_->GetHostHeader().empty() || request_->GetHeaders().empty()) {
//     return false;
//   }
//   return true;
// }
// bool HTTPRequestParser::IsFillBody() {
//   if (request_->GetBody().empty()) return false;
//   return true;
// }
// bool HTTPRequestParser::IsNeedBody() {
//   if ((request_->GetHeaders().count("Content-Length") > 0) ||
//       (request_->GetHeaders().count("Transfer-Encoding") > 0))
//     return true;
//   return false;
// }
//  リクエストの1行目のパース
//  もう少し綺麗に書けるかも
int HTTPRequestParser::SetRequestLine() {
  std::string request_line = row_line_;
  std::string method, uri, protocol, version;
  size_t pos = 0;
  // method
  pos = request_line.find(" ");
  if (pos == std::string::npos) {
    return kBadRequest;
  }
  method = request_line.substr(0, pos);
  request_line = request_line.substr(pos + 1);
  // uri
  pos = request_line.find(" ");
  if (pos == std::string::npos) {
    return kBadRequest;
  }
  uri = request_line.substr(0, pos);
  request_line = request_line.substr(pos + 1);
  // protocol
  pos = request_line.find("/");
  if (pos == std::string::npos) {
    return kBadRequest;
  }
  protocol = request_line.substr(0, pos);
  request_line = request_line.substr(pos + 1);
  // version
  pos = request_line.find("\r\n");
  if (pos == std::string::npos) {
    return kBadRequest;
  }
  version = request_line.substr(0, pos);
  row_line_ = request_line.substr(pos + 2);
  request_->SetMethod(str_toupper(method));
  request_->SetUri(str_toupper(uri));
  request_->SetProtocol(str_toupper(protocol));
  request_->SetVersion(str_toupper(version));
  return kOk;
}
