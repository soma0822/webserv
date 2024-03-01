#include "http_request.hpp"

HTTPRequest::HTTPRequest() {}
HTTPRequest::~HTTPRequest() {}

// request line setters
void HTTPRequest::SetMethod(const std::string &method) {
  this->method_ = method;
}
void HTTPRequest::SetUri(const std::string &uri) { this->uri_ = uri; }
void HTTPRequest::SetProtocol(const std::string &protocol) {
  this->protocol_ = protocol;
}
void HTTPRequest::SetVersion(const std::string &version) {
  this->version_ = version;
}

// header fields setters
void HTTPRequest::SetHostHeader(const std::string &host_header) {
  this->host_header_ = host_header;
}
void HTTPRequest::SetHeaders(
    const std::map<std::string, std::string> &headers) {
  this->headers_ = headers;
}

// body setters
void HTTPRequest::SetBody(const std::string &body) { this->body_ = body; }

// request line getters
std::string HTTPRequest::GetMethod() const { return this->method_; }
std::string HTTPRequest::GetUri() const { return this->uri_; }
std::string HTTPRequest::GetProtocol() const { return this->protocol_; }
std::string HTTPRequest::GetVersion() const { return this->version_; }

// header fields getters
std::string HTTPRequest::GetHostHeader() const { return this->host_header_; }
std::map<std::string, std::string> HTTPRequest::GetHeaders() const {
  return this->headers_;
}

// body getters
std::string HTTPRequest::GetBody() const { return this->body_; }
