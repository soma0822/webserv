#include "a_parser.hpp"

#include <cstddef>

// canonical
AParser::AParser()
    : request_(NULL), row_line_(""), parser_state_(kBeforeProcess) {}
AParser::~AParser() { delete request_; }
AParser::AParser(const AParser &other)
    : request_(NULL), row_line_(""), parser_state_(kBeforeProcess) {
  (void)other;
}
AParser &AParser::operator=(const AParser &other) {
  (void)other;
  return *this;
}

//  リクエストの1行目、もう少し綺麗に描きたい。
int AParser::SetRequestLine() {
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
  request_->SetMethod(string_utils::StrToUpper(method));
  request_->SetUri(string_utils::StrToUpper(uri));
  request_->SetProtocol(string_utils::StrToUpper(protocol));
  request_->SetVersion(string_utils::StrToUpper(version));
  return kOk;
}

// headerのパース
int AParser::SetRequestHeaders() {
  int status = SetHeader();
  if (status == kNotEnough)
    return kNotEnough;
  else if (status == kBadRequest)
    return kBadRequest;
  return CheckHeader();
}

int AParser::SetHeader() {
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
    request_->AddHeader(string_utils::StrToUpper(key), string_utils::StrToUpper(value));
  }
  // headerの終わりの確認
  row_line_ = request_line;
  // まだheaderが続いている場合
  if (request_line.find("\r\n") != 0) return kNotEnough;
  row_line_ = request_line.substr(2);
  return kOk;
}

int AParser::CheckHeader() {
  // Hostがあるか右辺が空白じゃないか確認
  if (request_->GetHeaders().count("HOST") == 0 ||
      request_->GetHeaders().find("HOST")->second == "")
    return kBadRequest;
  request_->SetHostHeader(request_->GetHeaders().find("HOST")->second);
  return kOk;
}

// bodyのパース
int AParser::SetRequestBody() {
  std::string request_line = row_line_;
  // content-length
  if (request_->GetHeaders().count("CONTENT-LENGTH") > 0) {
    // content-lengthの値を取得
    std::string length = request_->GetHeaders().find("CONTENT-LENGTH")->second;
    Result<int, std::string> result = string_utils::StrToI(length);
    if (result.IsErr()) return kBadRequest;
    // request_lineを取得して、長さの確認
    int pos = static_cast<int>(request_line.length());
    if (pos < result.Unwrap()) return kNotEnough;
    if (result.Unwrap() < pos) pos = result.Unwrap();
    row_line_ = request_line.substr(pos);
    request_->AddBody(request_line.substr(0, pos));
  }
  // trasfer-encoding
  else {
    return SetChunkedBody();
  }
  return kOk;
}

int AParser::SetChunkedBody() {
  static int chunked_state = kNeedChunkedSize;
  static size_t chunked_size = 0;
  size_t pos = 0;

  while (1) {
    // sizeが書かれているか確認
    if (chunked_state == kNeedChunkedSize) {
      pos = row_line_.find("\r\n");
      if (pos == 0) return kBadRequest;
      if (pos == std::string::npos) return kNotEnough;
      Result<int, std::string> result =
          string_utils::StrToHex(row_line_.substr(0, pos));
      if (result.IsErr()) return BadChunkedBody(chunked_state, chunked_size);
      chunked_size = static_cast<size_t>(result.Unwrap());
      row_line_ = row_line_.substr(pos + 2);
      chunked_state = kNeedChunkedBody;
    }
    // sizeの分だけbodyがあるか確認
    if (chunked_state == kNeedChunkedBody) {
      pos = row_line_.find("\r\n");
      if (pos == std::string::npos) return kNotEnough;
      // size == 0の時はすぐに\r\nが来て終わる
      if (pos == 0 && chunked_size == 0) {
        row_line_ = row_line_.substr(chunked_size + 2);
        chunked_state = kNeedChunkedSize;
        return kOk;
      } else if (pos == chunked_size) {
        request_->AddBody(row_line_.substr(0, chunked_size));
        row_line_ = row_line_.substr(chunked_size + 2);
        chunked_state = kNeedChunkedSize;
      } else
        return BadChunkedBody(chunked_state, chunked_size);
    }
  }
}

int AParser::SetBody() {
  request_->AddBody(row_line_);
  return kOk;
}

const Result<HTTPRequest *, int> AParser::BadRequest() {
  parser_state_ = kBeforeProcess;
  row_line_ = "";  // 一旦リセット
  delete request_;
  request_ = NULL;
  return Err(kBadRequest);
}

const Result<HTTPRequest *, int> AParser::OkRequest() {
  parser_state_ = kBeforeProcess;
  HTTPRequest *request = request_;
  request_ = NULL;
  return Ok(request);
}

int AParser::BadChunkedBody(int &chunked_state, size_t &chunked_size) {
  chunked_size = 0;
  chunked_state = kNeedChunkedSize;
  return kBadRequest;
}

bool AParser::IsChunked() {
  if ((request_->GetHeaders().count("TRANSFER-ENCODING") > 0) &&
      (request_->GetHeaders().find("TRANSFER-ENCODING")->second == "CHUNKED"))
    return true;
  return false;
}

bool AParser::IsContentLength() {
  if (request_->GetHeaders().count("CONTENT-LENGTH") > 0) return true;
  return false;
}

bool AParser::CheckNeedBodyHeader() {
  if (IsChunked() && IsContentLength()) return false;
  if (IsChunked() || IsContentLength())
    parser_state_ = kNeedBody;
  else
    parser_state_ = kBeforeProcess;
  return true;
}
