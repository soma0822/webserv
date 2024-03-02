#include "http_status_code.hpp"

namespace http {
std::string GetStatusMessage(StatusCode status_code) {
  switch (status_code) {
    case kContinue:
      return "Continue";
    case kSwitchingProtocols:
      return "Switching Protocols";
    case kOk:
      return "OK";
    case kCreated:
      return "Created";
    case kAccepted:
      return "Accepted";
    case kNonAuthoritativeInformation:
      return "Non-Authoritative Information";
    case kNoContent:
      return "No Content";
    case kResetContent:
      return "Reset Content";
    case kPartialContent:
      return "Partial Content";
    case kMultipleChoices:
      return "Multiple Choices";
    case kMovedPermanently:
      return "Moved Permanently";
    case kFound:
      return "Found";
    case kSeeOther:
      return "See Other";
    case kNotModified:
      return "Not Modified";
    case kUseProxy:
      return "Use Proxy";
    case kTemporaryRedirect:
      return "Temporary Redirect";
    case kPermanentRedirect:
      return "Permanent Redirect";
    case kBadRequest:
      return "Bad Request";
    case kUnauthorized:
      return "Unauthorized";
    case kPaymentRequired:
      return "Payment Required";
    case kForbidden:
      return "Forbidden";
    case kNotFound:
      return "Not Found";
    case kMethodNotAllowed:
      return "Method Not Allowed";
    case kNotAcceptable:
      return "Not Acceptable";
    case kProxyAuthenticationRequired:
      return "Proxy Authentication Required";
    case kRequestTimeout:
      return "Request Timeout";
    case kConflict:
      return "Conflict";
    case kGone:
      return "Gone";
    case kLengthRequired:
      return "Length Required";
    case kPreconditionFailed:
      return "Precondition Failed";
    case kPayloadTooLarge:
      return "Payload Too Large";
    case kUriTooLong:
      return "URI Too Long";
    case kUnsupportedMediaType:
      return "Unsupported Media Type";
    case kRangeNotSatisfiable:
      return "Range Not Satisfiable";
    case kExpectationFailed:
      return "Expectation Failed";
    case kMisdirectedRequest:
      return "Misdirected Request";
    case kUnprocessableEntity:
      return "Unprocessable Entity";
    case kUpgradeRequired:
      return "Upgrade Required";
    case kInternalServerError:
      return "Internal Server Error";
    case kNotImplemented:
      return "Not Implemented";
    case kBadGateway:
      return "Bad Gateway";
    case kServiceUnavailable:
      return "Service Unavailable";
    case kGatewayTimeout:
      return "Gateway Timeout";
    case kHttpVersionNotSupported:
      return "HTTP Version Not Supported";
  }
  return "";
}
}  // namespace http
