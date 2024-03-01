#include "http_request.hpp"

HttpRequest::HttpRequest() {}
HttpRequest::~HttpRequest() {}

// request line setters
void HttpRequest::SetMethod(const std::string &method) {
  this->method_ = method;
}
void HttpRequest::SetUri(const std::string &uri) { this->uri_ = uri; }
void HttpRequest::SetProtocol(const std::string &protocol) {
  this->protocol_ = protocol;
}
void HttpRequest::SetVersion(const std::string &version) {
  this->version_ = version;
}

// header fields setters
void HttpRequest::SetHostHeader(const std::string &host_header) {
  this->host_header_ = host_header;
}
void HttpRequest::SetHeaders(
    const std::map<std::string, std::string> &headers) {
  this->headers_ = headers;
}

// body setters
void HttpRequest::SetBody(const std::string &body) { this->body_ = body; }

// request line getters
std::string HttpRequest::GetMethod() const { return this->method_; }
std::string HttpRequest::GetUri() const { return this->uri_; }
std::string HttpRequest::GetProtocol() const { return this->protocol_; }
std::string HttpRequest::GetVersion() const { return this->version_; }

// header fields getters
std::string HttpRequest::GetHostHeader() const { return this->host_header_; }
std::map<std::string, std::string> HttpRequest::GetHeaders() const {
  return this->headers_;
}

// body getters
std::string HttpRequest::GetBody() const { return this->body_; }
