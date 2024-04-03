#include "a_parser.hpp"

#include "logger.hpp"
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

// request_lineのパース
int AParser::SetRequestLine() {
  size_t pos = row_line_.find("\r\n");
  if (pos == std::string::npos) return kNotEnough;
  std::string request_line = row_line_.substr(0, pos);
  row_line_ = row_line_.substr(pos + 2);
  std::pair<std::string, int> result;

  // method
  result = ParsePart(request_line, " ", kBadRequest);
  if (result.second != kOk) return result.second;
  if (string_utils::IsStrUpper(result.first) == false) return kBadRequest;
  request_->SetMethod(result.first);

  // uri
  request_line = string_utils::SkipSpace(request_line);
  result = ParsePart(request_line, " ", kBadRequest);
  if (result.second != kOk) return result.second;
  if (result.first[0] != '/') return kBadRequest;
  pos = result.first.find("?");
  if (pos != std::string::npos) {
    request_->SetUri(result.first.substr(0, pos));
    request_->SetQuery(result.first.substr(pos + 1));
  } else {
    request_->SetUri(result.first);
  }

  // protocol
  request_line = string_utils::SkipSpace(request_line);
  result = ParsePart(request_line, "/", kBadRequest);
  if (result.second != kOk) return result.second;
  request_->SetProtocol(string_utils::StrToUpper(result.first));

  // version
  if (request_line == "") return kBadRequest;
  request_->SetVersion(request_line);

  return CheckProtocol();
}

int AParser::CheckProtocol() {
  if (request_->GetProtocol() != "HTTP") return kBadRequest;
  if (request_->GetVersion() != "1.1") return kHttpVersionNotSupported;
  return kOk;
}

std::pair<std::string, int> AParser::ParsePart(std::string &str,
                                               const std::string &delimiter,
                                               int errorcode) {
  size_t pos = str.find(delimiter);
  if (pos == std::string::npos || pos == 0)
    return std::make_pair("", errorcode);
  std::string part = str.substr(0, pos);
  str = str.substr(pos + delimiter.size());
  return std::make_pair(part, kOk);
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
    if (key_pos == std::string::npos || key_pos > request_line.find("\r\n"))
      break;
    // どちらかの辺が空白、もしくは右辺だけない。
    if (value_pos == 0 || key_pos == 0) return kBadRequest;
    key = request_line.substr(0, key_pos);
    value =
        string_utils::SkipSpace(request_line.substr(key_pos + 1, value_pos));
    request_line = request_line.substr(key_pos + value_pos + 3);
    request_->AddHeader(string_utils::StrToUpper(key), value);
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
  if (request_->GetHeaders().count("HOST") == 0) return kBadRequest;
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
    if (result.IsErr() || kMaxBodySize < result.Unwrap()) {
      if (!validation::IsNumber(length))
        return kBadRequest;
      else
        return kPayloadTooLarge;
    }
    // request_lineを取得して、長さの確認
    int pos = static_cast<int>(request_line.length());
    if (pos < result.Unwrap()) return kNotEnough;
    if (result.Unwrap() < pos) pos = result.Unwrap();
    row_line_ = request_line.substr(pos);
    request_->AddBody(request_line.substr(0, pos));
  }
  // trasfer-encoding
  else {
    int ret = SetChunkedBody();
    Logger::Info() << "ret: " << ret << std::endl;
  }
  return kOk;
}

int AParser::SetChunkedBody() {
  static ChunkedState status = {kNeedChunkedSize, 0, 0};
  size_t pos = 0;
  Logger::Info() << "row_line_: " << row_line_ << std::endl;
  while (1) {
    // sizeが書かれているか確認
    if (status.chunked_state == kNeedChunkedSize) {
      pos = row_line_.find("\r\n");
      if (pos == 0) return ResetChunkedBody(status, kBadRequest);
      if (pos == std::string::npos) return kNotEnough;
      Result<int, std::string> result =
          string_utils::StrToHex(row_line_.substr(0, pos));
      if (result.IsErr() ||
          kMaxBodySize < result.Unwrap() + status.total_size) {
        if (!validation::IsHexNumber(row_line_.substr(0, pos)))
          return ResetChunkedBody(status, kBadRequest);
        else
          return ResetChunkedBody(status, kPayloadTooLarge);
      }
      status.chunked_size = static_cast<size_t>(result.Unwrap());
      status.total_size += status.chunked_size;
      row_line_ = row_line_.substr(pos + 2);
      status.chunked_state = kNeedChunkedBody;
    }
    if (status.chunked_state == kNeedChunkedBody) {
      pos = row_line_.find("\r\n");
      if (pos == std::string::npos) return kNotEnough;
      // size == 0の時はすぐに\r\nが来て終わる
      if (pos == 0 && status.chunked_size == 0) {
        row_line_ = row_line_.substr(status.chunked_size + 2);
        for (unsigned int i = 0; i < row_line_.size(); i++){
          Logger::Debug() << static_cast<int>(row_line_[i]) << std::endl;
         }
        return ResetChunkedBody(status, kOk);
      } else if (status.chunked_size < pos) {
        return ResetChunkedBody(status, kBadRequest);
      } else {
        request_->AddBody(row_line_.substr(0, pos));
        row_line_ = row_line_.substr(pos + 2);
        status.chunked_size -= pos;
        if (0 < status.chunked_size) return kNotEnough;
        status.chunked_state = kNeedChunkedSize;
      }
    }
  }
}

int AParser::SetBody() {
  request_->AddBody(row_line_);
  return kOk;
}

const Result<HTTPRequest *, int> AParser::ErrRequest(int status_code) {
  parser_state_ = kBeforeProcess;
  row_line_ = "";  // 一旦リセット
  delete request_;
  request_ = NULL;
  return Err(status_code);
}

const Result<HTTPRequest *, int> AParser::OkRequest() {
  parser_state_ = kBeforeProcess;
  HTTPRequest *request = request_;
  request_ = NULL;
  return Ok(request);
}

int AParser::ResetChunkedBody(ChunkedState &state, int status_code) {
  state.chunked_state = kNeedChunkedSize;
  state.chunked_size = 0;
  state.total_size = 0;
  return status_code;
}

bool AParser::IsChunked() {
  if (request_->GetHeaders().count("TRANSFER-ENCODING") > 0) return true;
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
