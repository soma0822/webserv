#include "http_response.hpp"

http::HTTPResponse::HTTPResponse() {}

http::HTTPResponse::HTTPResponse(const HTTPResponse &other)
    : status_code_(other.status_code_),
      headers_(other.headers_),
      body_(other.body_) {}

http::HTTPResponse &http::HTTPResponse::operator=(const HTTPResponse &other) {
  if (this == &other) {
    return *this;
  }
  status_code_ = other.status_code_;
  headers_ = other.headers_;
  body_ = other.body_;
  return *this;
}

http::HTTPResponse::~HTTPResponse() {}

void http::HTTPResponse::SetStatusCode(http::StatusCode status_code) {
  status_code_ = status_code;
}

http::StatusCode http::HTTPResponse::GetStatusCode() const {
  return status_code_;
}

const std::string &http::HTTPResponse::GetBody() const { return body_; }

void http::HTTPResponse::SetHeader(const std::string &key,
                                   const std::string &value) {
  headers_[key] = value;
}

std::string http::HTTPResponse::GetHeader(const std::string &key) {
  return headers_[key];
}

void http::HTTPResponse::SetBody(const std::string &body) { body_ = body; }

std::string http::HTTPResponse::ToString() const {
  return "HTTP/1.1 200 OK\n\nHello, World!";
}
