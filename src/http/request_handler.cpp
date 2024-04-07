#include "request_handler.hpp"

#include <dirent.h>
#include <unistd.h>

#include "config.hpp"
#include "file_utils.hpp"

const std::string RequestHandler::server_dir = "./www/html";

Option<HTTPResponse *> RequestHandler::Handle(const IConfig &config,
                                              RequestContext req_ctx) {
  HTTPRequest *request = req_ctx.request;
  if (!request) {
    return Some(GenerateErrorResponse(http::kInternalServerError, config));
  }
  const IServerContext &server_ctx =
      config.SearchServer(req_ctx.port, req_ctx.ip, request->GetHostHeader());
  const LocationContext &location_ctx =
      server_ctx.SearchLocation(request->GetUri());
  if (!location_ctx.GetReturn().empty()) {
    return Some(HTTPResponse::Builder()
                    .SetStatusCode(http::kMovedPermanently)
                    .AddHeader("Location", location_ctx.GetReturn())
                    .Build());
  }

  // リクエストボディのサイズが制限を超えている場合には413を返す
  if (static_cast<int>(request->GetBody().length()) >
      location_ctx.GetLimitClientBody()) {
    return Some(GenerateErrorResponse(http::kPayloadTooLarge, config));
  }

  // URIの長さが制限を超えている場合には414を返す
  if (request->GetUri().length() >= kMaxUriLength) {
    return Some(GenerateErrorResponse(http::kUriTooLong, config));
  }
  if (request->GetMethod() == "GET") {
    return Get(config, req_ctx);
  }
  if (request->GetMethod() == "POST") {
    return Post(config, req_ctx);
  }
  if (request->GetMethod() == "DELETE") {
    return Delete(config, req_ctx);
  }
  return Some(GenerateErrorResponse(http::kNotImplemented, config));
}

Option<HTTPResponse *> RequestHandler::Get(const IConfig &config,
                                           RequestContext req_ctx) {
  const HTTPRequest *request = req_ctx.request;
  const IServerContext &server_ctx =
      config.SearchServer(req_ctx.port, req_ctx.ip, request->GetHostHeader());
  const LocationContext &location_ctx =
      server_ctx.SearchLocation(request->GetUri());

  bool need_autoindex = false;
  std::pair<std::string, std::string> resolve_pair =
      ResolveRequestTargetPath(config, req_ctx);
  std::string request_file_path = resolve_pair.first;
  std::string path_translated = resolve_pair.second;
  Logger::Info() << request_file_path << ":" << path_translated << std::endl;
  if (file_utils::IsDirectory(request_file_path)) {
    // リクエストターゲットのディレクトリが/で終わっていない場合には301を返す
    if (request_file_path.at(request_file_path.size() - 1) != '/') {
      return Some(HTTPResponse::Builder()
                      .SetStatusCode(http::kMovedPermanently)
                      .AddHeader("Location", request->GetUri() + "/")
                      .Build());
    }
    // AutoIndexが有効な場合にはディレクトリの中身を表示する
    if (location_ctx.GetCanAutoIndex()) {
      // パーミッションを確認してからAutoIndexを表示する
      need_autoindex = true;
    }
  }

  if (need_autoindex) {
    // ファイルが存在しない場合には404を返す
    if (!file_utils::DoesFileExist(request_file_path)) {
      return Some(GenerateErrorResponse(http::kNotFound, config));
    }
    // パーミッションがない場合には403を返す
    if (!file_utils::IsReadable(request_file_path)) {
      return Some(GenerateErrorResponse(http::kForbidden, config));
    }
    // 許可されていないメソッドの場合には405を返す
    if (!IsAllowedMethod(config, req_ctx)) {
      return Some(GenerateErrorResponse(http::kMethodNotAllowed, config));
    }
    return Some(GenerateAutoIndexPage(config, request, request_file_path));
  }
  // リクエストされたファイルのパスがディレクトリの場合には、indexファイルが存在する場合にはそれを返す
  if (file_utils::IsDirectory(request_file_path)) {
    if (!location_ctx.GetIndex().empty()) {
      request_file_path += location_ctx.GetIndex();
    } else if (!server_ctx.GetIndex().empty()) {
      request_file_path += server_ctx.GetIndex();
    }
  }
  bool is_cgi = (request_file_path.rfind('.') != std::string::npos &&
                 location_ctx.IsValidCgiExtension(
                     request_file_path.substr(request_file_path.rfind('.'))));
  // ファイルが存在しない場合には404を返す
  if (!file_utils::DoesFileExist(request_file_path)) {
    return Some(GenerateErrorResponse(http::kNotFound, config));
  }
  // パーミッションがない場合には403を返す
  if ((!file_utils::IsReadable(request_file_path) && !is_cgi) ||
      (!file_utils::IsExecutable(request_file_path) && is_cgi)) {
    return Some(GenerateErrorResponse(http::kForbidden, config));
  }
  // 許可されていないメソッドの場合には405を返す
  if (!IsAllowedMethod(config, req_ctx)) {
    return Some(GenerateErrorResponse(http::kMethodNotAllowed, config));
  }
  if (is_cgi) {
    http::StatusCode status =
        CGIExe(config, req_ctx, request_file_path, path_translated);
    if (status == http::kOk)
      return None<HTTPResponse *>();
    else
      return Some(GenerateErrorResponse(status, config));
  }
  return Some(HTTPResponse::Builder()
                  .SetStatusCode(http::kOk)
                  .SetBody(file_utils::ReadFile(request_file_path))
                  .Build());
}

Option<HTTPResponse *> RequestHandler::Post(const IConfig &config,
                                            RequestContext req_ctx) {
  const HTTPRequest *request = req_ctx.request;
  const std::string &uri = request->GetUri();
  const IServerContext &server_ctx =
      config.SearchServer(req_ctx.port, req_ctx.ip, request->GetHostHeader());
  const LocationContext &location_ctx =
      server_ctx.SearchLocation(request->GetUri());

  std::pair<std::string, std::string> resolve_pair =
      ResolveRequestTargetPath(config, req_ctx);
  std::string request_file_path = resolve_pair.first;
  std::string path_translated = resolve_pair.second;
  Logger::Info() << request_file_path << " : " << path_translated << std::endl;
  // リクエストされたファイルのパスがディレクトリの場合には、indexファイルが存在する場合にはそれを返す
  std::string cgi_script;
  if (file_utils::IsDirectory(request_file_path)) {
    if (!location_ctx.GetIndex().empty()) {
      cgi_script = request_file_path + location_ctx.GetIndex();
    } else if (!server_ctx.GetIndex().empty()) {
      cgi_script = request_file_path + server_ctx.GetIndex();
    }
  } else {
    cgi_script = request_file_path;
  }
  bool is_cgi = (cgi_script.rfind('.') != std::string::npos &&
                 location_ctx.IsValidCgiExtension(
                     cgi_script.substr(cgi_script.rfind('.'))));
  // リクエストターゲットがディレクトリの場合には400を返す
  if (!is_cgi && request_file_path.at(request_file_path.size() - 1) == '/') {
    return Some(GenerateErrorResponse(http::kBadRequest, config));
  }
  const std::string parent_dir =
      request_file_path.substr(0, request_file_path.find_last_of('/'));
  // 親ディレクトリが存在しない場合には404を返す
  if (!file_utils::DoesFileExist(parent_dir)) {
    return Some(GenerateErrorResponse(http::kNotFound, config));
  }
  bool parent_unwritable = !is_cgi &&
                           !file_utils::DoesFileExist(request_file_path) &&
                           (!file_utils::IsWritable(parent_dir));
  bool request_file_unwritable = !is_cgi &&
                                 file_utils::IsFile(request_file_path) &&
                                 (!file_utils::IsWritable(request_file_path));
  // 親ディレクトリに書き込み権限がない場合には403を返す cgiの場合は実行権限
  if (parent_unwritable || request_file_unwritable ||
      (is_cgi && !file_utils::IsExecutable(cgi_script))) {
    return Some(GenerateErrorResponse(http::kForbidden, config));
  }
  // 許可されていないメソッドの場合には405を返す
  if (!IsAllowedMethod(config, req_ctx)) {
    return Some(GenerateErrorResponse(http::kMethodNotAllowed, config));
  }
  if (is_cgi) {
    http::StatusCode status =
        CGIExe(config, req_ctx, request_file_path, path_translated);
    if (status == http::kOk)
      return None<HTTPResponse *>();
    else
      return Some(GenerateErrorResponse(status, config));
  }

  std::ofstream ofs(request_file_path.c_str());
  if (!ofs) {
    return Some(GenerateErrorResponse(http::kInternalServerError, config));
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
  const std::string &uri = request->GetUri();

  std::pair<std::string, std::string> resolve_pair =
      ResolveRequestTargetPath(config, req_ctx);

  std::string request_file_path = resolve_pair.first;
  std::string path_translated = resolve_pair.second;

  // リクエストターゲットがディレクトリの場合には400を返す
  if (uri.at(uri.size() - 1) == '/' ||
      file_utils::IsDirectory(request_file_path)) {
    return Some(GenerateErrorResponse(http::kBadRequest, config));
  }

  // ファイルが存在しない場合には404を返す
  if (!file_utils::DoesFileExist(request_file_path) ||
      !path_translated.empty()) {
    Logger::Debug() << __LINE__ << " : " << request_file_path << " : " << (file_utils::DoesFileExist(request_file_path) == true ? "は存在します" : "は存在しません") << std::endl;
    return Some(GenerateErrorResponse(http::kNotFound, config));
  }

  const std::string parent_dir =
      request_file_path.substr(0, request_file_path.find_last_of('/'));
  // 親ディレクトリに書き込み権限がない場合には403を返す
  if (!file_utils::IsWritable(parent_dir)) {
    return Some(GenerateErrorResponse(http::kForbidden, config));
  }

  // 許可されていないメソッドの場合には405を返す
  if (!IsAllowedMethod(config, req_ctx)) {
    return Some(GenerateErrorResponse(http::kMethodNotAllowed, config));
  }

  // ファイルを削除する
  if (unlink(request_file_path.c_str()) == -1) {
    return Some(GenerateErrorResponse(http::kInternalServerError, config));
  }

  return Some(GenerateErrorResponse(http::kOk, config));
}

std::string RequestHandler::ResolveRootPath(const IConfig &config,
                                            const RequestContext req_ctx) {
  const HTTPRequest *request = req_ctx.request;
  const IServerContext &server_ctx =
      config.SearchServer(req_ctx.port, req_ctx.ip, request->GetHostHeader());
  const std::string &uri = request->GetUri();
  const LocationContext &location_ctx = server_ctx.SearchLocation(uri);

  std::string root = server_ctx.GetRoot();
  if (!location_ctx.GetRoot().empty()) {
    root = location_ctx.GetRoot();
  }

  return root;
}

// uriの/cgi-bin/default.py/foo/barの時　pair<./www/cgi-bin/default.py,
// ./www/foo/bar> で返すようにした
std::pair<std::string, std::string> RequestHandler::ResolveRequestTargetPath(
    const IConfig &config, const RequestContext req_ctx) {
  // rootを取得する
  std::string root = ResolveRootPath(config, req_ctx);
  bool root_empty = root.empty();
  std::string uri = req_ctx.request->GetUri();
  std::string target_path;
  if (!root_empty && root.at(root.size() - 1) == '/') {
    root.erase(root.size() - 1, 1);
  }
  const HTTPRequest *request = req_ctx.request;
  const IServerContext &server_ctx =
      config.SearchServer(req_ctx.port, req_ctx.ip, request->GetHostHeader());
  const LocationContext &location_ctx = server_ctx.SearchLocation(uri);
  // RFC9112によれば、OPTIONSとCONNECT以外のリクエストはパスが以下の形式になる
  // origin-form = absolute-path [ "?" query ]
  // rootが/で終わっている場合には/が重複してしまうので削除する
  if (root_empty) {
    target_path = server_dir + uri;
  } else {
    uri = RemoveLocPath(config, req_ctx);
    target_path = root + uri;
  }
  Logger::Debug() << root << " : uri : " << uri << std::endl;
  std::string script_path = ResolveScriptPart(location_ctx, target_path);
  std::string path_translated = target_path.substr(script_path.length());
  if (!path_translated.empty()) {
    if (root_empty) {
      path_translated = server_dir + target_path.substr(script_path.length());
    } else {
      path_translated = root + target_path.substr(script_path.length());
    }
  }
  return std::make_pair(script_path, path_translated);
}

// 途中でファイルがあり、その拡張子がcgiのものであればそれを返す。なければそのまま返す
std::string RequestHandler::ResolveScriptPart(const LocationContext &loc_ctx,
                                              const std::string &target) {
  unsigned long pos = 0;
  while ((pos = target.find("/", pos + 1)) != std::string::npos) {
    std::string tmp = target.substr(0, pos);
    if (file_utils::IsFile(tmp) &&
        loc_ctx.IsValidCgiExtension(tmp.substr(tmp.rfind(".")))) {
      return tmp;
    }
  }
  return target;
}

// uriからLocationのPath部分を消したものを返す
std::string RequestHandler::RemoveLocPath(const IConfig &config,
                                          const RequestContext req_ctx) {
  const HTTPRequest *request = req_ctx.request;
  const IServerContext &server_ctx =
      config.SearchServer(req_ctx.port, req_ctx.ip, request->GetHostHeader());
  const std::string &uri = request->GetUri();
  const LocationContext &location_ctx = server_ctx.SearchLocation(uri);
  std::string loc_path = location_ctx.GetPath();
  if (loc_path.length() != 0 && loc_path.at(loc_path.size() - 1) == '/') {
    loc_path.erase(loc_path.size() - 1, 1);
  }
  return uri.substr(loc_path.length());
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
    body += "<li><a href=\"" + request->GetUri() + dp->d_name + "\">" +
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
    perror("execve");
    Logger::Error() << "execve エラー" << std::endl;
    std::exit(1);
  }
  close(cgi_fd[1]);
  if (req_ctx.request->GetMethod() == "POST") close(redirect_fd[0]);
  IOTaskManager::AddTask(new ReadFromCGI(pid, cgi_fd[0], req_ctx, config,
                                         time_utils::GetCurrentTime()));
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
  env_map["REMOTE_ADDR"] =
      addr_utils::InetNtoa(req_ctx.client_addr.sin_addr.s_addr);
  env_map["SCRIPT_NAME"] = script_name;
  std::string host = req_ctx.request->GetHostHeader();
  if (host.find(":") != std::string::npos) {
    env_map["SERVER_NAME"] = host.substr(0, host.find(":"));
  } else {
    env_map["SERVER_NAME"] = host;
  }
  env_map["SERVER_PORT"] = req_ctx.port;
  env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
  env_map["SERVER_SOFTWARE"] = "webserv";
  // Cookieの取得
  env_map["COOKIE"] = req_ctx.request->GetHeaders().count("COOKIE") == 1
                          ? req_ctx.request->GetHeaders().find("COOKIE")->second
                          : "";
  return env_map;
}

size_t RequestHandler::CountEnv(char **env) {
  size_t count = 0;
  while (env[count] != NULL) {
    ++count;
  }
  return count;
}

char **RequestHandler::DupEnv(
    const std::map<std::string, std::string> &env_map) {
  extern char **environ;
  char **env = new char *[env_map.size() + CountEnv(environ) + 1];

  Logger::Debug() << "-------env_map--------" << std::endl;
  std::map<std::string, std::string>::const_iterator it = env_map.begin();
  for (unsigned int i = 0; it != env_map.end(); ++it, ++i) {
    std::string tmp = it->first + "=" + it->second;
    env[i] = new char[tmp.size() + 1];
    std::strcpy(env[i], tmp.c_str());
    Logger::Debug() << env[i] << std::endl;
  }
  for (unsigned int i = 0; environ[i] != NULL; ++i) {
    env[env_map.size() + i] = new char[std::strlen(environ[i]) + 1];
    std::strcpy(env[env_map.size() + i], environ[i]);
    Logger::Debug() << env[env_map.size() + i] << std::endl;
  }
  Logger::Debug() << "----------------------" << std::endl;
  env[env_map.size() + CountEnv(environ)] = NULL;
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
  const LocationContext &location_ctx = server_ctx.SearchLocation(uri);

  // cgi extensionが0より大きければCGIリクエストである
  return (location_ctx.GetCgiExtension().size() > 0);
}

bool RequestHandler::IsAllowedMethod(const IConfig &config,
                                     RequestContext req_ctx) {
  const HTTPRequest *request = req_ctx.request;
  const IServerContext &server_ctx =
      config.SearchServer(req_ctx.port, req_ctx.ip, request->GetHostHeader());
  const std::string &uri = request->GetUri();
  const LocationContext &location_ctx = server_ctx.SearchLocation(uri);
  return location_ctx.IsAllowedMethod(request->GetMethod());
}

RequestHandler::RequestHandler() {}

RequestHandler::RequestHandler(const RequestHandler &other) { (void)other; }

RequestHandler &RequestHandler::operator=(const RequestHandler &other) {
  (void)other;
  return *this;
}

RequestHandler::~RequestHandler() {}
