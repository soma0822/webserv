#include "http_request_parser.hpp"

#include <iostream>
#include <string>

#include "http_request.hpp"

std::string HTTPRequestParser::str_toupper(std::string s) {
  for (size_t i = 0; i < s.length(); i++) {
    s[i] = std::toupper(s[i]);
  }
  return s;
}

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

const Result<HTTPRequest *, int> HTTPRequestParser::Parser(
    std::string request_line) {
  if (request_ == NULL) request_ = new HTTPRequest();
  // newのエラー処理
  // requestlineの内容が全て埋まっているか確認
  if (IsFillRequestLine() == false) {
    if (SetRequestLine() == kBadRequest) {
      return Err(kBadRequest);
    }
  }
  if (IsFillHeaders() == false) {
    if (SetRequestHeaders() == kBadRequest) {
      return Err(kBadRequest);
    }
  }
  if (IsFillBody() == false &&
      (request_->GetHeaders().count("Content-Length") > 0 ||
       request_->GetHeaders().count("Transfer-Encoding") > 0)) {
    if (SetRequestBody() == kBodyNotEnough) {
      return Err(kBodyNotEnough);
    }
  }
  // setterと形式があってるかのエラーチェック
  return Ok(request_);
  (void)request_line;
}

//情報がぜんぶ埋まっているか確認
bool HTTPRequestParser::IsFillRequestLine() {
  return request_->GetMethod().empty() || request_->GetUri().empty() ||
         request_->GetProtocol().empty() || request_->GetVersion().empty();
}
bool HTTPRequestParser::IsFillHeaders() {
  return request_->GetHostHeader().empty() || request_->GetHeaders().empty();
}
bool HTTPRequestParser::IsFillBody() { return request_->GetBody().empty(); }

// リクエストの1行目のパース
// もう少し綺麗に書けるかも
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

// headerのパース
int HTTPRequestParser::SetRequestHeaders() {
  std::string request_line = row_line_;
  std::string key, value;
  size_t pos = 0;
  while (1) {
    pos = request_line.find(":");
    if (pos == std::string::npos) {
      // headerの終わりの確認
      if (request_line != "\r\n") {
        row_line_ = request_line;
        return kBadRequest;
      }
      // Hostがあるか確認
      if (request_->GetHeaders().count("Host") == 0) {
        row_line_ = request_line;
        return kBadRequest;
      }
      break;
    }
    key = request_line.substr(0, pos);
    request_line = request_line.substr(pos + 1);
    pos = request_line.find("\r\n");
    if (pos == std::string::npos) {
      break;
    }
    value = request_line.substr(0, pos);
    request_line = request_line.substr(pos + 2);
    request_->AddHeader(key, value);
  }
  row_line_ = request_line;
  return kOk;
}

// bodyのパース確認
int HTTPRequestParser::SetRequestBody() {
  std::string request_line = row_line_;
  size_t pos = 0;
  if (request_->GetHeaders().count("Content-Length") > 0) {
    pos = request_line.find("\r\n");
    if (pos == std::string::npos) {
      return kBodyNotEnough;
    }
    request_->SetBody(request_line.substr(0, pos));
    row_line_ = request_line.substr(pos + 2);
  } else if (request_->GetHeaders().count("Transfer-Encoding") > 0) {
    pos = request_line.find("\r\n");
    if (pos == std::string::npos) {
      return kBodyNotEnough;
    }
    request_->SetBody(request_line.substr(0, pos));
    row_line_ = request_line.substr(pos + 2);
  }
  return kOk;
}
