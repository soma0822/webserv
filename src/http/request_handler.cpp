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
  Result<std::string, HTTPResponse *> file_path =
      ResolvePath(server_context, request->GetUri());
  if (file_path.IsErr()) {
    return file_path.UnwrapErr();
  }
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

Result<std::string, HTTPResponse *> RequestHandler::ResolvePath(
    const IServerContext &server_ctx, const std::string &uri) {
  Result<LocationContext, std::string> location_ctx_result =
      server_ctx.SearchLocation(uri);

  // rootを取得する
  std::string root = location_ctx_result.IsOk()
                         ? location_ctx_result.Unwrap().GetRoot()
                         : server_ctx.GetRoot();

  // RFC9112によれば、OPTIONSとCONNECT以外のリクエストはパスが以下の形式になる
  // origin-form = absolute-path [ "?" query ]
  // rootが/で終わっている場合には/が重複してしまうので削除する
  if (root.at(root.size() - 1) == '/') {
    root.erase(root.size() - 1, 1);
  }

  // リクエストされたファイルのパスがディレクトリの場合には、indexファイルを返す
  std::string request_file_path = root + uri;
  if (file_utils::IsDirectory(request_file_path)) {
    if (request_file_path.at(request_file_path.size() - 1) != '/') {
      // 正しいuriを指定して301を返す
    }
    request_file_path += location_ctx_result.IsOk()
                             ? location_ctx_result.Unwrap().GetIndex()
                             : server_ctx.GetIndex();
  }

  return Ok(request_file_path);
}