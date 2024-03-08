#include "http_request_parser.hpp"

#include <cstddef>

// canonical
HTTPRequestParser::HTTPRequestParser()
    : request_(NULL), row_line_(""), parser_state_(kBeforeProcess) {}
HTTPRequestParser::~HTTPRequestParser() {}
HTTPRequestParser::HTTPRequestParser(const HTTPRequestParser &other) {
  (void)other;
}
HTTPRequestParser &HTTPRequestParser::operator=(
    const HTTPRequestParser &other) {
  (void)other;
  return *this;
}

// リクエストのパース
const Result<HTTPRequest *, int> HTTPRequestParser::Parser(
    std::string request_line) {
  row_line_ = row_line_ + request_line;
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
    int return_state = SetRequestHeaders();
    if (return_state == kBadRequest)
      return BadRequest();
    else if (return_state == kNotEnough)
      return Err(kNotEnough);
    else {
      if (IsNeedBody() == true) {
        parser_state_ = kNeedBody;
      } else {
        parser_state_ = kBeforeProcess;
      }
    }
  }
  // bodyの内容を確認
  if (parser_state_ == kNeedBody) {
    if (SetRequestBody() == kNotEnough) {
      return Err(kNotEnough);
    }
  }
  return OkRequest();
}

//  リクエストの1行目、もう少し綺麗に描きたい。
int HTTPRequestParser::SetRequestLine() {
  std::string request_line = row_line_;
  std::string method, uri, protocol, version;
  size_t pos = 0;
  // method
  pos = request_line.find(" ");
  if (pos == std::string::npos || pos == 0) {
    return kBadRequest;
  }
  method = request_line.substr(0, pos);
  request_line = request_line.substr(pos + 1);
  // uri
  pos = request_line.find(" ");
  if (pos == std::string::npos || pos == 0) {
    return kBadRequest;
  }
  uri = request_line.substr(0, pos);
  request_line = request_line.substr(pos + 1);
  // protocol
  pos = request_line.find("/");
  if (pos == std::string::npos || pos == 0) {
    return kBadRequest;
  }
  protocol = request_line.substr(0, pos);
  request_line = request_line.substr(pos + 1);
  // version
  pos = request_line.find("\r\n");
  if (pos == std::string::npos || pos == 0) {
    return kBadRequest;
  }
  version = request_line.substr(0, pos);
  row_line_ = request_line.substr(pos + 2);
  request_->SetMethod(StrToUpper(method));
  request_->SetUri(StrToUpper(uri));
  request_->SetProtocol(StrToUpper(protocol));
  request_->SetVersion(StrToUpper(version));
  return kOk;
}

// headerのパース
int HTTPRequestParser::SetRequestHeaders() {
  std::string request_line = row_line_;
  std::string key, value;
  size_t key_pos = 0;
  size_t value_pos = 0;
  while (1) {
    key_pos = request_line.find(":");
    value_pos = request_line.find("\r\n") - key_pos - 1;
    // 最後まで見てしまった時
    if (key_pos == std::string::npos) break;
    // どちらかの辺が空白、もしくは右辺だけない。
    if (value_pos == 0 || key_pos == 0) return kBadRequest;
    key = request_line.substr(0, key_pos);
    value =
        string_utils::SkipSpace(request_line.substr(key_pos + 1, value_pos));
    request_line = request_line.substr(key_pos + value_pos + 3);
    request_->AddHeader(StrToUpper(key), StrToUpper(value));
  }
  // headerの終わりの確認
  // まだheaderが続いている場合
  if (request_line.find("\r\n") != 0) {
    row_line_ = request_line;
    return kNotEnough;
  }
  // Hostがあるか右辺が空白じゃないか確認
  if (request_->GetHeaders().count("HOST") == 0 ||
      request_->GetHeaders().find("HOST")->second == "") {
    row_line_ = request_line.substr(2);
    return kBadRequest;
  }
  request_->SetHostHeader(request_->GetHeaders().find("HOST")->second);
  row_line_ = request_line.substr(2);
  return kOk;
}

// bodyのパース
int HTTPRequestParser::SetRequestBody() {
  std::string request_line = row_line_;
  size_t pos = 0;

  if (request_->GetHeaders().count("CONTENT-LENGTH") > 0) {
    pos = request_line.find("\r\n");
    if (pos == std::string::npos) pos = request_line.length();
    std::string length = request_->GetHeaders().find("CONTENT-LENGTH")->second;
    Result<int, std::string> result = string_utils::StrToI(length);
    if (result.IsErr()) return kBadRequest;
    if (static_cast<int>(request_line.length()) < result.Unwrap())
      return kNotEnough;
    else {
      row_line_ = request_line.substr(pos);
      request_->AddBody(request_line.substr(0, pos));
    }
  }
  // trasfer-encodingは未実装
  // 数字以外きたらbadrequest
  // 数字の値と違ったらbadrequest
  else {
    return SetChunkedBody();
  }
  return kOk;
}

int HTTPRequestParser::BadChunkedBody(int &chunked_state,
                                      size_t &chunked_size) {
  chunked_size = 0;
  chunked_state = kNeedChunkedSize;
  return kBadRequest;
}

int HTTPRequestParser::SetChunkedBody() {
  static int chunked_state = kNeedChunkedSize;
  static size_t chunked_size = 0;
  size_t pos = 0;

  while (1) {
    if (chunked_state == kNeedChunkedSize) {
      pos = row_line_.find("\r\n");
      if (pos == std::string::npos) return kNotEnough;
      Result<int, std::string> result =
          string_utils::StrToI(row_line_.substr(0, pos));
      if (result.IsErr()) return BadChunkedBody(chunked_state, chunked_size);
      chunked_size = static_cast<size_t>(result.Unwrap());
      row_line_ = row_line_.substr(pos + 2);
      chunked_state = kNeedChunkedBody;
    }
    if (chunked_state == kNeedChunkedBody) {
      pos = row_line_.find("\r\n");
      if (pos == std::string::npos) return kNotEnough;
      if (pos == 0 && chunked_size == 0) {
        row_line_ = row_line_.substr(chunked_size + 2);
        chunked_state = kNeedChunkedSize;
        return kOk;
      } else if (pos == chunked_size) {
        request_->AddBody(row_line_.substr(0, chunked_size));
        row_line_ = row_line_.substr(chunked_size + 2);
        chunked_state = kNeedChunkedEnd;
      } else
        return BadChunkedBody(chunked_state, chunked_size);
    }
    if (chunked_state == kNeedChunkedEnd) {
      pos = row_line_.find("\r\n");
      if (pos == std::string::npos) return kNotEnough;
      row_line_ = row_line_.substr(2);
      if (pos == 0)
        chunked_state = kNeedChunkedSize;
      else
        return BadChunkedBody(chunked_state, chunked_size);
    }
  }
}

// utils
std::string HTTPRequestParser::StrToUpper(std::string s) {
  for (size_t i = 0; i < s.length(); i++) {
    s[i] = std::toupper(s[i]);
  }
  return s;
}

const Result<HTTPRequest *, int> HTTPRequestParser::BadRequest() {
  parser_state_ = kBeforeProcess;
  row_line_ = "";  // 一旦リセット
  delete request_;
  request_ = NULL;
  return Err(kBadRequest);
}

const Result<HTTPRequest *, int> HTTPRequestParser::OkRequest() {
  parser_state_ = kBeforeProcess;
  HTTPRequest *request = request_;
  request_ = NULL;
  return Ok(request);
}

bool HTTPRequestParser::IsNeedBody() {
  if ((request_->GetHeaders().count("CONTENT-LENGTH") > 0) ||
      (request_->GetHeaders().find("TRANSFER-ENCODING")->second == "CHUNKED"))
    return true;
  return false;
}
