#include "http_response.hpp"

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

void HTTPResponse::SetStatusCode(http::StatusCode status_code) {
  status_code_ = status_code;
}

http::StatusCode HTTPResponse::GetStatusCode() const { return status_code_; }

const std::string &HTTPResponse::GetBody() const { return body_; }

void HTTPResponse::AddHeader(const std::string &key, const std::string &value) {
  headers_[key] = value;
}

const std::map<std::string, std::string> &HTTPResponse::GetHeaders() const {
  return headers_;
}

void HTTPResponse::SetBody(const std::string &body) { body_ = body; }

std::string HTTPResponse::ToString() const {
  std::map<http::StatusCode, std::string>::const_iterator it =
      http::kStatusMessage.find(status_code_);
  return "HTTP/1.1 " + it->second + " OK\n\nHello, World!";
}
