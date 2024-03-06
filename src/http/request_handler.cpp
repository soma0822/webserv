#include "request_handler.hpp"

#include "config.hpp"
#include "file_utils.hpp"

const HTTPResponse *RequestHandler::Handle(const HTTPRequest *request,
                                           const std::string &port) {
  (void)request;
  (void)port;
  return new HTTPResponse();
}

const HTTPResponse *RequestHandler::Get(const HTTPRequest *request,
                                        const std::string &port) {
  ServerContext server = Config::SearchServer(port, request->GetHostHeader());
  // TODO Locationからパスを取得する
  const std::string requested_file_path = server.GetRoot() + request->GetUri();

  HTTPResponse *response = new HTTPResponse();
  // TODO あとでHTTP1.1を定数化する
  response->SetHttpVersion("HTTP/1.1");
  response->SetStatusCode(http::kOk);
  Result<std::string, file_utils::Error> file_content =
      file_utils::ReadFile(requested_file_path);
  if (file_content.IsOk()) {
    response->SetBody(file_content.Unwrap());
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