#include "request_handler.hpp"

#include "config.hpp"
#include "file_utils.hpp"

HTTPResponse *RequestHandler::Handle(const IConfig &config,
                                     const HTTPRequest *request,
                                     const std::string &port,
                                     const std::string &ip) {
  if (!request) {
    // 505を返す
  }
  (void)config;
  (void)port;
  (void)ip;
  return new HTTPResponse();
}

HTTPResponse *RequestHandler::Get(const HTTPRequest *request,
                                  const std::string &requested_file_path) {
  if (!request) {
    // 505を返す
  }
  // TODO リクエストのヘッダを処理する

  HTTPResponse *response = new HTTPResponse();
  response->SetHTTPVersion("HTTP/1.1");
  response->SetStatusCode(http::kOk);
  Result<std::string, file_utils::Error> file_content =
      file_utils::ReadFile(requested_file_path);
  response->SetBody(file_content.Unwrap());
  if (file_content.IsErr()) {
    // TODO 404を返す
  }

  return response;
}

RequestHandler::RequestHandler() {}

RequestHandler::RequestHandler(const RequestHandler &other) { (void)other; }

RequestHandler &RequestHandler::operator=(const RequestHandler &other) {
  (void)other;
  return *this;
}

RequestHandler::~RequestHandler() {}