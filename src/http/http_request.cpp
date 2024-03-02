#include "http_request.hpp"

HTTPRequest::HTTPRequest() {}
HTTPRequest::HTTPRequest(const HTTPRequest &other)
    : port_(other.port_),
      method_(other.method_),
      uri_(other.uri_),
      protocol_(other.protocol_),
      version_(other.version_),
      host_header_(other.host_header_),
      headers_(other.headers_),
      body_(other.body_) {}
HTTPRequest &HTTPRequest::operator=(const HTTPRequest &other) {
  if (this != &other) {
    this->port_ = other.port_;
    this->method_ = other.method_;
    this->uri_ = other.uri_;
    this->protocol_ = other.protocol_;
    this->version_ = other.version_;
    this->host_header_ = other.host_header_;
    this->headers_ = other.headers_;
    this->body_ = other.body_;
  }
  return *this;
}
HTTPRequest::~HTTPRequest() {}

// port setter
void HTTPRequest::SetPort(const std::string &port) { port_ = port; }

// request line setters
void HTTPRequest::SetMethod(const std::string &method) { method_ = method; }
void HTTPRequest::SetUri(const std::string &uri) { uri_ = uri; }
void HTTPRequest::SetProtocol(const std::string &protocol) {
  protocol_ = protocol;
}
void HTTPRequest::SetVersion(const std::string &version) { version_ = version; }

// header fields setters
void HTTPRequest::SetHostHeader(const std::string &host_header) {
  host_header_ = host_header;
}
void HTTPRequest::AddHeader(const std::string &key, const std::string &value) {
  headers_[key] = value;
}

// body setters
void HTTPRequest::SetBody(const std::string &body) { body_ = body; }

// port getter
const std::string &HTTPRequest::GetPort() const { return port_; }

// request line getters
const std::string &HTTPRequest::GetMethod() const { return method_; }
const std::string &HTTPRequest::GetUri() const { return uri_; }
const std::string &HTTPRequest::GetProtocol() const { return protocol_; }
const std::string &HTTPRequest::GetVersion() const { return version_; }

// header fields getters
const std::string &HTTPRequest::GetHostHeader() const { return host_header_; }
const std::map<std::string, std::string> &HTTPRequest::GetHeaders() const {
  return headers_;
}

// body getters
const std::string &HTTPRequest::GetBody() const { return body_; }
