#ifndef WEBSERV_SRC_HTTP_STATUS_CODE_HPP
#define WEBSERV_SRC_HTTP_STATUS_CODE_HPP

#include <map>
#include <string>

namespace http {
enum StatusCode {
  kNone = 0,
  kContinue = 100,
  kSwitchingProtocols = 101,
  kOk = 200,
  kCreated = 201,
  kAccepted = 202,
  kNonAuthoritativeInformation = 203,
  kNoContent = 204,
  kResetContent = 205,
  kPartialContent = 206,
  kMultipleChoices = 300,
  kMovedPermanently = 301,
  kFound = 302,
  kSeeOther = 303,
  kNotModified = 304,
  kUseProxy = 305,
  kTemporaryRedirect = 307,
  kPermanentRedirect = 308,
  kBadRequest = 400,
  kUnauthorized = 401,
  kPaymentRequired = 402,
  kForbidden = 403,
  kNotFound = 404,
  kMethodNotAllowed = 405,
  kNotAcceptable = 406,
  kProxyAuthenticationRequired = 407,
  kRequestTimeout = 408,
  kConflict = 409,
  kGone = 410,
  kLengthRequired = 411,
  kPreconditionFailed = 412,
  kPayloadTooLarge = 413,
  kUriTooLong = 414,
  kUnsupportedMediaType = 415,
  kRangeNotSatisfiable = 416,
  kExpectationFailed = 417,
  kMisdirectedRequest = 421,
  kUnprocessableEntity = 422,
  kUpgradeRequired = 426,
  kInternalServerError = 500,
  kNotImplemented = 501,
  kBadGateway = 502,
  kServiceUnavailable = 503,
  kGatewayTimeout = 504,
  kHttpVersionNotSupported = 505,
};

std::string GetStatusMessage(StatusCode status_code);

}  // namespace http

#endif  // WEBSERV_SRC_HTTP_STATUS_CODE_HPP
