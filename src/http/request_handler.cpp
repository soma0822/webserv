#include "request_handler.hpp"

#include "config.hpp"
#include "file_utils.hpp"

HTTPResponse *RequestHandler::Handle(const IConfig &config,
                                     const HTTPRequest *request,
                                     const std::string &port,
                                     const std::string &ip) {
  if (!request) {
    // 500を返す
  }
  const ServerContext &server_context =
      config.SearchServer(port, ip, request->GetHostHeader());
  std::string requested_file_path =
      ResolvePath(server_context, request->GetUri());
  return new HTTPResponse();
}

HTTPResponse *RequestHandler::Get(const HTTPRequest *request,
                                  const std::string &requested_file_path) {
  if (!request) {
    // 500を返す
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

std::string RequestHandler::ResolvePath(const IServerContext &context,
                                        const std::string &uri) {
  const LocationContext &location_context = context.SearchLocation(uri);

  // rootの絶対パスを取得
  std::string root = context.GetRoot();
  if (location_context.GetRoot().empty()) {
    root = location_context.GetRoot();
  }

  return root + uri;
}