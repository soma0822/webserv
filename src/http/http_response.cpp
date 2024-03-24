#include "http_response.hpp"

#include <sys/stat.h>

#include <file_utils.hpp>
#include <sstream>

const std::string HTTPResponse::Builder::kHTTPVersion = "HTTP/1.1";

HTTPResponse::Builder::Builder() : response_(new HTTPResponse) {
  response_->SetStatusCode(http::kNone);
}

// response_はBuildした後に返すためdeleteしない
HTTPResponse::Builder::~Builder() {}

HTTPResponse::Builder &HTTPResponse::Builder::SetStatusCode(
    http::StatusCode status_code) {
  response_->SetStatusCode(status_code);
  return *this;
}

HTTPResponse::Builder &HTTPResponse::Builder::AddHeader(
    const std::string &key, const std::string &value) {
  response_->AddHeader(key, value);
  return *this;
}

HTTPResponse::Builder &HTTPResponse::Builder::SetBody(const std::string &body) {
  response_->SetBody(body);
  return *this;
}

HTTPResponse *HTTPResponse::Builder::Build() {
  response_->SetHTTPVersion(kHTTPVersion);
  if (response_->GetStatusCode() == http::kNone) {
    delete response_;
    return NULL;
  }
  return response_;
}

// copy constructor
HTTPResponse::Builder::Builder(const HTTPResponse::Builder &other)
    : response_(new HTTPResponse(*(other.response_))) {}

const HTTPResponse::Builder &HTTPResponse::Builder::operator=(
    const HTTPResponse::Builder &other) {
  if (this == &other) {
    return *this;
  }
  (void)other;
  return *this;
}

HTTPResponse::HTTPResponse() {}

HTTPResponse::HTTPResponse(const HTTPResponse &other)
    : status_code_(other.status_code_),
      headers_(other.headers_),
      body_(other.body_) {}

HTTPResponse &HTTPResponse::operator=(const HTTPResponse &other) {
  if (this == &other) {
    return *this;
  }
  status_code_ = other.status_code_;
  headers_ = other.headers_;
  body_ = other.body_;
  return *this;
}

HTTPResponse::~HTTPResponse() {}

void HTTPResponse::SetHTTPVersion(const std::string &http_version) {
  http_version_ = http_version;
}

const std::string &HTTPResponse::GetHTTPVersion() const {
  return http_version_;
}

void HTTPResponse::SetStatusCode(http::StatusCode status_code) {
  status_code_ = status_code;
}

http::StatusCode HTTPResponse::GetStatusCode() const { return status_code_; }

const std::string &HTTPResponse::GetBody() const { return body_; }

void HTTPResponse::AddHeader(const std::string &key, const std::string &value) {
  headers_[string_utils::StrToLower(key)] = value;
}

const std::map<std::string, std::string> &HTTPResponse::GetHeaders() const {
  return headers_;
}

void HTTPResponse::SetBody(const std::string &body) { body_ = body; }

std::string HTTPResponse::ToString() {
  std::ostringstream ss;

  // ステータスライン
  ss << http_version_ << " " << status_code_ << " "
     << GetStatusMessage(status_code_) << "\r\n";

  // ヘッダ
  for (std::map<std::string, std::string>::iterator it = headers_.begin();
       it != headers_.end(); ++it) {
    ss << string_utils::CapitalizeHyphenSeparatedWords(it->first) << ": "
       << it->second << "\r\n";
  }

  if (headers_.count("content-length") == 0) {
    ss << "Content-Length: " << body_.size() << "\r\n";
  }

  // 空行
  ss << "\r\n";

  // ボディ
  ss << body_;

  return ss.str();
}

HTTPResponse *GenerateErrorResponse(const http::StatusCode status_code,
                                    const IConfig &config) {
  const std::map<std::string, std::string> &error_pages = config.GetErrorPage();
  std::stringstream ss;
  ss << status_code;
  // エラーページが設定されていない場合はデフォルトのエラーページを返す
  if (error_pages.count(ss.str()) == 0) {
    return HTTPResponse::Builder()
        .SetStatusCode(status_code)
        .SetBody(GetErrorPage(status_code))
        .Build();
  }

  const std::string error_page_path = error_pages.at(ss.str());
  struct stat file_st;
  // エラーページが存在しない、または読み込み権限がない場合はデフォルトのエラーページを返す
  if (stat(error_page_path.c_str(), &file_st) == -1 ||
      S_ISDIR(file_st.st_mode) || !(file_st.st_mode & S_IRUSR)) {
    return HTTPResponse::Builder()
        .SetStatusCode(status_code)
        .SetBody(GetErrorPage(status_code))
        .Build();
  }

  return HTTPResponse::Builder()
      .SetStatusCode(status_code)
      .AddHeader("Content-Type", "text/html")
      .SetBody(file_utils::ReadFile(error_page_path))
      .Build();
}
