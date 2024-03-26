#include "request_handler.hpp"

#include <dirent.h>
#include <unistd.h>

#include "config.hpp"
#include "file_utils.hpp"

Option<HTTPResponse *> RequestHandler::Handle(const IConfig &config,
                                              RequestContext req_ctx) {
  HTTPRequest *request = req_ctx.request;
  if (!request) {
    return Some(GenerateErrorResponse(http::kInternalServerError, config));
  }
  const IServerContext &server_ctx =
      config.SearchServer(req_ctx.port, req_ctx.ip, request->GetHostHeader());
  const Result<LocationContext, std::string> location_ctx_result =
      server_ctx.SearchLocation(request->GetUri());

  // リクエストボディのサイズが制限を超えている場合には413を返す
  if (location_ctx_result.IsOk() &&
      static_cast<int>(request->GetBody().length()) >
          location_ctx_result.Unwrap().GetLimitClientBody()) {
    return Some(GenerateErrorResponse(http::kPayloadTooLarge, config));
  }

  // URIの長さが制限を超えている場合には414を返す
  if (request->GetUri().length() >= kMaxUriLength) {
    return Some(GenerateErrorResponse(http::kUriTooLong, config));
  }

  bool is_cgi_request = IsCGIRequest(config, req_ctx);

  if (!is_cgi_request && request->GetMethod() == "GET") {
    return Get(config, req_ctx);
  }
  if (!is_cgi_request && request->GetMethod() == "POST") {
    return Post(config, req_ctx);
  }
  if (!is_cgi_request && request->GetMethod() == "DELETE") {
    return Delete(config, req_ctx);
  }
  if (is_cgi_request) {
    // GETとPOST以外のメソッドはサポートしていない
    if (req_ctx.request->GetMethod() != "POST" &&
        req_ctx.request->GetMethod() != "GET") {
      return Some(GenerateErrorResponse(http::kNotImplemented, config));
    }

    const std::string cgi_script_abs_path =
        GetAbsoluteCGIScriptPath(config, req_ctx);
    const std::string cgi_script_path_segment =
        GetAbsolutePathForPathSegment(config, req_ctx);

    // CGIスクリプトの拡張子が許可されていない場合にはテキストとして返す
    bool is_valid_cgi_extension =
        location_ctx_result.IsOk() &&
        location_ctx_result.Unwrap().IsValidCgiExtension(
            cgi_script_abs_path.substr(cgi_script_abs_path.find('.')));
    bool is_method_allowed = IsAllowedMethod(config, req_ctx);
    if (!is_valid_cgi_extension && req_ctx.request->GetMethod() == "GET" &&
        is_method_allowed) {
      return Get(config, req_ctx);
    }

    http::StatusCode status =
        CGIExe(config, req_ctx, cgi_script_abs_path, cgi_script_path_segment);
    if (status == http::kOk) {
      return None<HTTPResponse *>();
    }
    return Some(GenerateErrorResponse(status, config));
  }
  return Some(GenerateErrorResponse(http::kNotImplemented, config));
}

Option<HTTPResponse *> RequestHandler::Get(const IConfig &config,
                                           RequestContext req_ctx) {
  const HTTPRequest *request = req_ctx.request;
  const IServerContext &server_ctx =
      config.SearchServer(req_ctx.port, req_ctx.ip, request->GetHostHeader());
  const Result<LocationContext, std::string> location_ctx_result =
      server_ctx.SearchLocation(request->GetUri());

  bool need_autoindex = false;

  std::string request_file_path = ResolveRequestTargetPath(config, req_ctx);
  if (file_utils::IsDirectory(request_file_path)) {
    // リクエストターゲットのディレクトリが/で終わっていない場合には301を返す
    if (request_file_path.at(request_file_path.size() - 1) != '/') {
      return Some(HTTPResponse::Builder()
                      .SetStatusCode(http::kMovedPermanently)
                      .AddHeader("Location", request->GetUri() + "/")
                      .Build());
    }
    // AutoIndexが有効な場合にはディレクトリの中身を表示する
    if (location_ctx_result.IsOk() &&
        location_ctx_result.Unwrap().GetCanAutoIndex()) {
      // パーミッションを確認してからAutoIndexを表示する
      need_autoindex = true;
    }
  }

  if (need_autoindex) {
    // ファイルが存在しない場合には404を返す
    if (!file_utils::DoesFileExist(request_file_path)) {
      return Some(
          HTTPResponse::Builder().SetStatusCode(http::kNotFound).Build());
    }
    // 許可されていないメソッドの場合には405を返す
    if (!IsAllowedMethod(config, req_ctx)) {
      return Some(GenerateErrorResponse(http::kMethodNotAllowed, config));
    }
    // パーミッションがない場合には403を返す
    if (!file_utils::IsReadable(request_file_path)) {
      return Some(
          HTTPResponse::Builder().SetStatusCode(http::kForbidden).Build());
    }
    return Some(GenerateAutoIndexPage(config, request, request_file_path));
  }

  // リクエストされたファイルのパスがディレクトリの場合には、indexファイルが存在する場合にはそれを返す
  if (file_utils::IsDirectory(request_file_path)) {
    if (location_ctx_result.IsOk() &&
        !location_ctx_result.Unwrap().GetIndex().empty()) {
      request_file_path += location_ctx_result.Unwrap().GetIndex();
    } else if (!server_ctx.GetIndex().empty()) {
      request_file_path += server_ctx.GetIndex();
    }
  }
  // ファイルが存在しない場合には404を返す
  if (!file_utils::DoesFileExist(request_file_path)) {
    return Some(HTTPResponse::Builder().SetStatusCode(http::kNotFound).Build());
  }
  // 許可されていないメソッドの場合には405を返す
  if (!IsAllowedMethod(config, req_ctx)) {
    return Some(GenerateErrorResponse(http::kMethodNotAllowed, config));
  }
  // パーミッションがない場合には403を返す
  if (!file_utils::IsReadable(request_file_path)) {
    return Some(
        HTTPResponse::Builder().SetStatusCode(http::kForbidden).Build());
  }

  return Some(HTTPResponse::Builder()
                  .SetStatusCode(http::kOk)
                  .SetBody(file_utils::ReadFile(request_file_path))
                  .Build());
}

Option<HTTPResponse *> RequestHandler::Post(const IConfig &config,
                                            RequestContext req_ctx) {
  const HTTPRequest *request = req_ctx.request;
  const IServerContext &server_ctx =
      config.SearchServer(req_ctx.port, req_ctx.ip, request->GetHostHeader());
  const std::string &uri = request->GetUri();
  const Result<LocationContext, std::string> location_ctx_result =
      server_ctx.SearchLocation(uri);

  const std::string request_file_path =
      ResolveRequestTargetPath(config, req_ctx);
  // リクエストターゲットがディレクトリの場合には400を返す
  if (request_file_path.at(request_file_path.size() - 1) == '/') {
    return Some(
        HTTPResponse::Builder().SetStatusCode(http::kBadRequest).Build());
  }

  const std::string parent_dir =
      request_file_path.substr(0, request_file_path.find_last_of('/'));
  // 親ディレクトリが存在しない場合には404を返す
  if (!file_utils::DoesFileExist(parent_dir)) {
    return Some(HTTPResponse::Builder().SetStatusCode(http::kNotFound).Build());
  }
  // 許可されていないメソッドの場合には405を返す
  if (!IsAllowedMethod(config, req_ctx)) {
    return Some(GenerateErrorResponse(http::kMethodNotAllowed, config));
  }
  // 親ディレクトリに書き込み権限がない場合には403を返す
  if (!file_utils::IsWritable(parent_dir)) {
    return Some(
        HTTPResponse::Builder().SetStatusCode(http::kForbidden).Build());
  }

  std::ofstream ofs(request_file_path.c_str());
  if (!ofs) {
    return Some(HTTPResponse::Builder()
                    .SetStatusCode(http::kInternalServerError)
                    .Build());
  }
  ofs << request->GetBody();
  ofs.close();

  return Some(HTTPResponse::Builder()
                  .SetStatusCode(http::kCreated)
                  .AddHeader("Location", uri)
                  .Build());
}

Option<HTTPResponse *> RequestHandler::Delete(const IConfig &config,
                                              RequestContext req_ctx) {
  const HTTPRequest *request = req_ctx.request;
  const IServerContext &server_ctx =
      config.SearchServer(req_ctx.port, req_ctx.ip, request->GetHostHeader());
  const std::string &uri = request->GetUri();
  const Result<LocationContext, std::string> location_ctx_result =
      server_ctx.SearchLocation(uri);

  const std::string request_file_path =
      ResolveRequestTargetPath(config, req_ctx);

  // リクエストターゲットがディレクトリの場合には400を返す
  if (uri.at(uri.size() - 1) == '/' ||
      file_utils::IsDirectory(request_file_path)) {
    return Some(
        HTTPResponse::Builder().SetStatusCode(http::kBadRequest).Build());
  }

  // ファイルが存在しない場合には404を返す
  if (!file_utils::DoesFileExist(request_file_path)) {
    return Some(HTTPResponse::Builder().SetStatusCode(http::kNotFound).Build());
  }

  // 許可されていないメソッドの場合には405を返す
  if (!IsAllowedMethod(config, req_ctx)) {
    return Some(GenerateErrorResponse(http::kMethodNotAllowed, config));
  }

  const std::string parent_dir =
      request_file_path.substr(0, request_file_path.find_last_of('/'));
  // 親ディレクトリに書き込み権限がない場合には403を返す
  if (!file_utils::IsWritable(parent_dir)) {
    return Some(
        HTTPResponse::Builder().SetStatusCode(http::kForbidden).Build());
  }

  // ファイルを削除する
  if (unlink(request_file_path.c_str()) == -1) {
    return Some(HTTPResponse::Builder()
                    .SetStatusCode(http::kInternalServerError)
                    .Build());
  }

  return Some(HTTPResponse::Builder().SetStatusCode(http::kOk).Build());
}

std::string RequestHandler::ResolveAbsoluteRootPath(const IConfig &config,
                                                    RequestContext req_ctx) {
  const HTTPRequest *request = req_ctx.request;
  const IServerContext &server_ctx =
      config.SearchServer(req_ctx.port, req_ctx.ip, request->GetHostHeader());
  const std::string &uri = request->GetUri();
  const Result<LocationContext, std::string> location_ctx_result =
      server_ctx.SearchLocation(uri);

  std::string root = server_ctx.GetRoot();
  if (location_ctx_result.IsOk() &&
      !location_ctx_result.Unwrap().GetRoot().empty()) {
    root = location_ctx_result.Unwrap().GetRoot();
  }

  return root;
}

std::string RequestHandler::ResolveRequestTargetPath(
    const IConfig &config, const RequestContext req_ctx) {
  // rootを取得する
  std::string root = ResolveAbsoluteRootPath(config, req_ctx);

  // RFC9112によれば、OPTIONSとCONNECT以外のリクエストはパスが以下の形式になる
  // origin-form = absolute-path [ "?" query ]
  // rootが/で終わっている場合には/が重複してしまうので削除する
  if (root.at(root.size() - 1) == '/') {
    root.erase(root.size() - 1, 1);
  }
  return root + req_ctx.request->GetUri();
}

std::string RequestHandler::GetAbsoluteCGIScriptPath(const IConfig &config,
                                                     RequestContext req_ctx) {
  std::string request_file_path = ResolveRequestTargetPath(config, req_ctx);
  // 拡張子以降のパスセグメントは除外する
  size_t pos_period = request_file_path.find('.', 1);
  size_t pos_separator = request_file_path.substr(pos_period).find('/');
  // '/'が見つからない場合には拡張子以降のパスセグメントがないのでそのまま返す
  if (pos_separator == std::string::npos) {
    return request_file_path;
  }
  return request_file_path.substr(0, pos_period + pos_separator);
}

// 存在しない場合は空文字を返す
std::string RequestHandler::GetAbsolutePathForPathSegment(
    const IConfig &config, RequestContext req_ctx) {
  std::string request_file_path = ResolveRequestTargetPath(config, req_ctx);

  // CGIスクリプトの絶対パス以降がパスセグメントになる
  std::string path_segment = request_file_path.substr(
      GetAbsoluteCGIScriptPath(config, req_ctx).size());
  if (path_segment.empty()) {
    return "";
  }
  return ResolveAbsoluteRootPath(config, req_ctx) + path_segment;
}

HTTPResponse *RequestHandler::GenerateAutoIndexPage(
    const IConfig &config, const HTTPRequest *request,
    const std::string &abs_path) {
  DIR *dir = opendir(abs_path.c_str());
  if (!dir) {
    return GenerateErrorResponse(http::kInternalServerError, config);
  }

  dirent *dp;
  std::string body =
      "<html><head><title>AutoIndex</title></head><body><h1>AutoIndex</h1><ul>";
  errno = 0;
  while ((dp = readdir(dir)) != NULL) {
    body += "<li><a href=\"" + request->GetUri() + "/" + dp->d_name + "\">" +
            dp->d_name + "</a></li>";
  }
  if (errno != 0) {
    closedir(dir);
    return GenerateErrorResponse(http::kInternalServerError, config);
  }
  body += "</ul></body></html>";
  closedir(dir);

  return HTTPResponse::Builder().SetStatusCode(http::kOk).SetBody(body).Build();
}
// ex.) script_name = /cgi-bin/default.py
// path_translated:パスセグメントの絶対パス
http::StatusCode RequestHandler::CGIExe(const IConfig &config,
                                        RequestContext req_ctx,
                                        const std::string &script_name,
                                        const std::string &path_translated) {
  // スクリプトが存在しない場合には404を返す
  if (!file_utils::DoesFileExist(script_name)) {
    return http::kNotFound;
  }

  // 許可されていないメソッドの場合には405を返す
  if (!IsAllowedMethod(config, req_ctx)) {
    return http::kMethodNotAllowed;
  }

  // スクリプトが実行可能でない場合には403を返す
  if (file_utils::IsExecutable(script_name)) {
    return http::kForbidden;
  }

  int redirect_fd[2], cgi_fd[2];
  pid_t pid;
  if (pipe(cgi_fd) == -1) {
    Logger::Error() << "pipe エラー" << std::endl;
    return http::kInternalServerError;
  }
  if (fcntl(cgi_fd[0], F_SETFL, O_NONBLOCK) == -1) {
    Logger::Error() << "fcntl エラー" << std::endl;
    close(cgi_fd[0]);
    close(cgi_fd[1]);
    return http::kInternalServerError;
  }
  if (req_ctx.request->GetMethod() == "POST") {
    if (pipe(redirect_fd) == -1) {
      Logger::Error() << "pipe エラー" << std::endl;
      close(cgi_fd[0]);
      close(cgi_fd[1]);
      return http::kInternalServerError;
    }
    if (fcntl(redirect_fd[1], F_SETFL, O_NONBLOCK) == -1) {
      Logger::Error() << "fcntl エラー" << std::endl;
      close(cgi_fd[0]);
      close(cgi_fd[1]);
      close(redirect_fd[0]);
      close(redirect_fd[1]);
      return http::kInternalServerError;
    }
    IOTaskManager::AddTask(
        new WriteToCGI(redirect_fd[1], req_ctx.request->GetBody()));
    Logger::Info() << "WriteToCGIを追加" << std::endl;
  }
  pid = fork();
  if (pid == -1) {
    Logger::Error() << "fork エラー" << std::endl;
    if (req_ctx.request->GetMethod() == "POST") {
      close(redirect_fd[0]);
      close(redirect_fd[1]);
    }
    close(cgi_fd[0]);
    close(cgi_fd[1]);
    return http::kInternalServerError;
  }
  if (pid == 0) {
    close(cgi_fd[0]);
    dup2(cgi_fd[1], 1);
    close(cgi_fd[1]);
    std::map<std::string, std::string> env_map =
        GetEnv(config, req_ctx, script_name, path_translated);
    char **env = DupEnv(env_map);
    if (env_map["REQUEST_METHOD"] == "POST") {
      close(redirect_fd[1]);
      dup2(redirect_fd[0], 0);
      close(redirect_fd[0]);
    }
    const char *argv[] = {script_name.c_str(), NULL};
    Logger::Info() << "CGI実行: " << argv[0] << std::endl;
    execve(argv[0], const_cast<char *const *>(argv), env);
    Logger::Error() << "execve エラー" << std::endl;
    std::exit(1);
  }
  close(cgi_fd[1]);
  if (req_ctx.request->GetMethod() == "POST") close(redirect_fd[0]);
  IOTaskManager::AddTask(new ReadFromCGI(pid, cgi_fd[0], req_ctx, config));
  Logger::Info() << "ReadFromCGIを追加" << std::endl;
  return http::kOk;
}

std::map<std::string, std::string> RequestHandler::GetEnv(
    const IConfig &config, const RequestContext &req_ctx,
    const std::string &script_name, const std::string &path_translated) {
  std::map<std::string, std::string> env_map;
  (void)config;
  const HTTPRequest *req = req_ctx.request;
  const std::map<std::string, std::string> &headers = req->GetHeaders();
  env_map["REQUEST_METHOD"] = req->GetMethod();
  if (headers.find("Authorization") != headers.end())
    env_map["AUTH_TYPE"] = headers.find("Authorization")->second;
  else
    env_map["AUTH_TYPE"] = "";
  if (env_map["REQUEST_METHOD"] == "POST") {
    unsigned int size = req->GetBody().size();
    std::stringstream ss;
    ss << size;
    env_map["CONTENT_LENGTH"] = ss.str();
  }
  if (headers.find("CONTENT-TYPE") != headers.end())
    env_map["CONTENT_TYPE"] = headers.find("CONTENT-TYPE")->second;
  else
    env_map["CONTENT_TYPE"] = "";
  env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
  env_map["PATH_INFO"] = req->GetUri();
  env_map["PATH_TRANSLATED"] = path_translated;
  env_map["QUERY_STRING"] = req->GetQuery();
  // TODO: inet_ntoa使用禁止なので自作
  env_map["REMOTE_ADDR"] = inet_ntoa(req_ctx.client_addr.sin_addr);
  // REMOTE_HOSTは使用可能関数ではわからない
  env_map["REMOTE_HOST"] = "";
  // TODO: userの取得
  if (env_map["AUTH_TYPE"] == "")
    env_map["REMOTE_USER"] = "";
  else
    env_map["REMOTE_USER"] = "user";
  env_map["SCRIPT_NAME"] = script_name;
  // TODO: URIからサーバ名の取得
  env_map["SERVER_NAME"] = "localhost";
  env_map["SERVER_PORT"] = req_ctx.port;
  env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
  env_map["SERVER_SOFTWARE"] = "webserv";
  return env_map;
}

char **RequestHandler::DupEnv(
    const std::map<std::string, std::string> &env_map) {
  char **env = new char *[env_map.size() + 1];
  Logger::Info() << "-------env_map--------" << env_map.size() << std::endl;
  std::map<std::string, std::string>::const_iterator it = env_map.begin();
  for (unsigned int i = 0; it != env_map.end(); ++it, ++i) {
    std::string tmp = it->first + "=" + it->second;
    env[i] = new char[tmp.size() + 1];
    std::strcpy(env[i], tmp.c_str());
    Logger::Info() << env[i] << std::endl;
  }
  return env;
}

void RequestHandler::DeleteEnv(char **env) {
  if (env == NULL) return;
  for (unsigned int i = 0; env[i] != NULL; ++i) {
    delete[] env[i];
  }
  delete[] env;
}

bool RequestHandler::IsCGIRequest(const IConfig &config,
                                  RequestContext req_ctx) {
  const HTTPRequest *request = req_ctx.request;
  const IServerContext &server_ctx =
      config.SearchServer(req_ctx.port, req_ctx.ip, request->GetHostHeader());
  const std::string &uri = request->GetUri();
  const Result<LocationContext, std::string> location_ctx_result =
      server_ctx.SearchLocation(uri);

  // cgi extensionが0より大きければCGIリクエストである
  return location_ctx_result.IsOk() &&
         location_ctx_result.Unwrap().GetCgiExtension().size() > 0;
}

bool RequestHandler::IsAllowedMethod(const IConfig &config,
                                     RequestContext req_ctx) {
  const HTTPRequest *request = req_ctx.request;
  const IServerContext &server_ctx =
      config.SearchServer(req_ctx.port, req_ctx.ip, request->GetHostHeader());
  const std::string &uri = request->GetUri();
  const Result<LocationContext, std::string> location_ctx_result =
      server_ctx.SearchLocation(uri);
  return location_ctx_result.IsOk() &&
         location_ctx_result.Unwrap().IsAllowedMethod(request->GetMethod());
}

const char **RequestHandler::MakeArgv(const std::string &script_name,
                                      std::string &first_line) {
  const char **ret;
  if (first_line[0] == '#' && first_line[1] == '!') {
    if (first_line.find("#!/usr/bin/env") == 0) {
      char *path = getenv("PATH");
      std::stringstream ss(first_line);
      std::string exec_filename;
      ss >> exec_filename;
      ss >> exec_filename;
      std::istringstream path_stream(path);
      std::string dir;
      std::string program_path;
      while (std::getline(path_stream, dir, ':')) {
        std::string tmp = dir + "/" + exec_filename;
        if (access(tmp.c_str(), X_OK) == 0) {
          program_path = tmp;
          break;
        }
      }
      if (program_path.empty()) std::exit(1);
      first_line = program_path;
    } else {
      first_line = first_line.substr(2);
    }
    ret = const_cast<const char **>(new char *[3]);
    ret[0] = first_line.c_str();
    ret[1] = script_name.c_str();
    ret[2] = NULL;
  } else {
    ret = const_cast<const char **>(new char *[2]);
    ret[0] = script_name.c_str();
    ret[1] = NULL;
  }
  return ret;
}

RequestHandler::RequestHandler() {}

RequestHandler::RequestHandler(const RequestHandler &other) { (void)other; }

RequestHandler &RequestHandler::operator=(const RequestHandler &other) {
  (void)other;
  return *this;
}

RequestHandler::~RequestHandler() {}
