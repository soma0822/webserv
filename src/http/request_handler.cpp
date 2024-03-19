#include "request_handler.hpp"

#include <sys/stat.h>
#include <unistd.h>

#include "config.hpp"
#include "file_utils.hpp"

HTTPResponse *RequestHandler::Handle(const IConfig &config,
                                     const HTTPRequest *request,
                                     const std::string &port,
                                     const std::string &ip) {
  if (!request) {
    return HTTPResponse::Builder()
        .SetStatusCode(http::kInternalServerError)
        .Build();
  }
  if (request->GetUri().length() >= kMaxUriLength) {
    return GenerateErrorResponse(http::kUriTooLong, config);
  }
  if (request->GetMethod() == "GET") {
    return Get(config, request, port, ip);
  }
  if (request->GetMethod() == "POST") {
    return Post(config, request, port, ip);
  }
  if (request->GetMethod() == "DELETE") {
    return Delete(config, request, port, ip);
  }
  return GenerateErrorResponse(http::kNotImplemented, config);
}

HTTPResponse *RequestHandler::Get(const IConfig &config,
                                  const HTTPRequest *request,
                                  const std::string &port,
                                  const std::string &ip) {
  const IServerContext &server_ctx =
      config.SearchServer(port, ip, request->GetHostHeader());
  const std::string &uri = request->GetUri();
  const Result<LocationContext, std::string> location_ctx_result =
      server_ctx.SearchLocation(uri);

  // rootを取得する
  std::string root = server_ctx.GetRoot();
  if (location_ctx_result.IsOk() &&
      !location_ctx_result.Unwrap().GetRoot().empty()) {
    root = location_ctx_result.Unwrap().GetRoot();
  }

  // RFC9112によれば、OPTIONSとCONNECT以外のリクエストはパスが以下の形式になる
  // origin-form = absolute-path [ "?" query ]
  // rootが/で終わっている場合には/が重複してしまうので削除する
  if (root.at(root.size() - 1) == '/') {
    root.erase(root.size() - 1, 1);
  }

  // リクエストされたファイルのパスがディレクトリの場合には、indexファイルが存在する場合にはそれを返す
  std::string request_file_path = root + uri;
  if (file_utils::IsDirectory(request_file_path)) {
    if (location_ctx_result.IsOk() &&
        !location_ctx_result.Unwrap().GetIndex().empty()) {
      request_file_path += location_ctx_result.Unwrap().GetIndex();
    } else if (!server_ctx.GetIndex().empty()) {
      request_file_path += server_ctx.GetIndex();
    }
  }

  if (file_utils::IsDirectory(request_file_path)) {
    // リクエストターゲットのディレクトリが/で終わっていない場合には301を返す
    if (request_file_path.at(request_file_path.size() - 1) != '/') {
      return HTTPResponse::Builder()
          .SetStatusCode(http::kMovedPermanently)
          .AddHeader("Location", request->GetUri() + "/")
          .Build();
    }
    // AutoIndexが有効な場合にはディレクトリの中身を表示する
    if (location_ctx_result.IsOk() &&
        location_ctx_result.Unwrap().GetCanAutoIndex()) {
      // TODO: AutoIndex
    }
  }

  struct stat file_stat;
  // ファイルが存在しない場合には404を返す
  if (stat(request_file_path.c_str(), &file_stat) == -1) {
    return HTTPResponse::Builder().SetStatusCode(http::kNotFound).Build();
  }
  // パーミッションがない場合には403を返す
  if (!(file_stat.st_mode & S_IRUSR)) {
    return HTTPResponse::Builder().SetStatusCode(http::kForbidden).Build();
  }

  return HTTPResponse::Builder()
      .SetStatusCode(http::kOk)
      .SetBody(file_utils::ReadFile(request_file_path))
      .Build();
}

HTTPResponse *RequestHandler::Post(const IConfig &config,
                                   const HTTPRequest *request,
                                   const std::string &port,
                                   const std::string &ip) {
  const IServerContext &server_ctx =
      config.SearchServer(port, ip, request->GetHostHeader());
  const std::string &uri = request->GetUri();
  const Result<LocationContext, std::string> location_ctx_result =
      server_ctx.SearchLocation(uri);

  // rootを取得する
  std::string root = server_ctx.GetRoot();
  if (location_ctx_result.IsOk() &&
      !location_ctx_result.Unwrap().GetRoot().empty()) {
    root = location_ctx_result.Unwrap().GetRoot();
  }

  // rootが/で終わっている場合には/が重複してしまうので削除する
  if (root.at(root.size() - 1) == '/') {
    root.erase(root.size() - 1, 1);
  }

  const std::string request_file_path = root + uri;
  // リクエストターゲットがディレクトリの場合には400を返す
  if (request_file_path.at(request_file_path.size() - 1) == '/') {
    return HTTPResponse::Builder().SetStatusCode(http::kBadRequest).Build();
  }

  const std::string parent_dir =
      request_file_path.substr(0, request_file_path.find_last_of('/'));
  struct stat parent_dir_stat;
  // 親ディレクトリが存在しない場合には404を返す
  if (stat(parent_dir.c_str(), &parent_dir_stat) == -1) {
    return HTTPResponse::Builder().SetStatusCode(http::kNotFound).Build();
  }
  // 親ディレクトリに書き込み権限がない場合には403を返す
  if (!(parent_dir_stat.st_mode & S_IWUSR)) {
    return HTTPResponse::Builder().SetStatusCode(http::kForbidden).Build();
  }

  std::ofstream ofs(request_file_path.c_str());
  if (!ofs) {
    return HTTPResponse::Builder()
        .SetStatusCode(http::kInternalServerError)
        .Build();
  }
  ofs << request->GetBody();
  ofs.close();

  return HTTPResponse::Builder()
      .SetStatusCode(http::kCreated)
      .AddHeader("Location", uri)
      .Build();
}

HTTPResponse *RequestHandler::Delete(const IConfig &config,
                                     const HTTPRequest *request,
                                     const std::string &port,
                                     const std::string &ip) {
  const IServerContext &server_ctx =
      config.SearchServer(port, ip, request->GetHostHeader());
  const std::string &uri = request->GetUri();
  const Result<LocationContext, std::string> location_ctx_result =
      server_ctx.SearchLocation(uri);

  // rootを取得する
  std::string root = server_ctx.GetRoot();
  if (location_ctx_result.IsOk() &&
      !location_ctx_result.Unwrap().GetRoot().empty()) {
    root = location_ctx_result.Unwrap().GetRoot();
  }

  // rootが/で終わっている場合には/が重複してしまうので削除する
  if (root.at(root.size() - 1) == '/') {
    root.erase(root.size() - 1, 1);
  }

  const std::string request_file_path = root + uri;

  // リクエストターゲットがディレクトリの場合には400を返す
  if (uri.at(uri.size() - 1) == '/' || file_utils::IsDirectory(root + uri)) {
    return HTTPResponse::Builder().SetStatusCode(http::kBadRequest).Build();
  }

  // ファイルが存在しない場合には404を返す
  struct stat file_stat;
  if (stat(request_file_path.c_str(), &file_stat) == -1) {
    return HTTPResponse::Builder().SetStatusCode(http::kNotFound).Build();
  }

  // パーミッションがない場合には403を返す
  if (!(file_stat.st_mode & S_IWUSR) && !(file_stat.st_mode & S_IXUSR)) {
    return HTTPResponse::Builder().SetStatusCode(http::kForbidden).Build();
  }

  // ファイルを削除する
  if (unlink(request_file_path.c_str()) == -1) {
    return HTTPResponse::Builder()
        .SetStatusCode(http::kInternalServerError)
        .Build();
  }

  return HTTPResponse::Builder().SetStatusCode(http::kOk).Build();
}

RequestHandler::RequestHandler() {}

RequestHandler::RequestHandler(const RequestHandler &other) { (void)other; }

RequestHandler &RequestHandler::operator=(const RequestHandler &other) {
  (void)other;
  return *this;
}

RequestHandler::~RequestHandler() {}
