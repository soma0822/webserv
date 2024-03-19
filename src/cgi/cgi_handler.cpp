#include "cgi_handler.hpp"

void CgiHandler::Handle(HTTPRequest *req, int client_fd, const IConfig &config,
                        const std::string &port, const std::string &ip) {
  std::string body = req->GetBody();
  int fd[2];
  int fd2[2];
  pid_t pid;
  std::map<std::string, std::string> env_map = GetEnv(req, config, port, ip);
  char **env = DupEnv(env_map);
  if (env == NULL) {
    Logger::Error() << "envの作成エラー" << std::endl;
    return;
  }
  if (pipe(fd) == -1) {
    Logger::Error() << "pipe エラー" << std::endl;
    return;
  }
  if (fcntl(fd[0], F_SETFL, O_NONBLOCK) == -1) {
    Logger::Error() << "fcntl エラー" << std::endl;
    return;
  }
  if (env_map["REQUEST_METHOD"] == "POST"){
    if (pipe(fd2) == -1) {
      Logger::Error() << "pipe エラー" << std::endl;
      return;
    } 
    if (fcntl(fd2[1], F_SETFL, O_NONBLOCK) == -1) {
      Logger::Error() << "fcntl エラー" << std::endl;
      return;
    }
    IOTaskManager::AddTask(new WriteToCGI(fd2[1], body));
    Logger::Info() << "WriteToCGIを追加" << std::endl;
  }
  pid = fork();
  if (pid == -1) {
    Logger::Error() << "fork エラー" << std::endl;
    return;
  }
  if (pid == 0) {
    close(fd[0]);
    dup2(fd[1], 1);
    if (env_map["REQUEST_METHOD"] == "POST"){
      close(fd2[1]);
     dup2(fd2[0], 0);
    }
    std::stringstream ss;
    ss << "Content-length=" << body.size();
    std::string content_length = ss.str();
    char cwd[1024];
    getcwd(cwd, 1024);
    std::string cgi_file;
    if (env_map["REQUEST_METHOD"] == "POST")
      cgi_file = cwd + std::string("/cgi-bin/default.py");
    else
      cgi_file = cwd + std::string("/cgi-bin/get.py");
    const char *argv[] = {"/usr/bin/python3",
                          cgi_file.c_str(),
                          NULL};
    Logger::Info() << "CGI実行" << std::endl;
    execve("/usr/bin/python3", const_cast<char *const *>(argv), env);
  }
  close(fd[1]);
  if (env_map["REQUEST_METHOD"] == "POST")
    close(fd2[0]);
  for (unsigned int i = 0; i < env_map.size(); ++i) {
    delete[] env[i];
  }
  delete[] env;
  IOTaskManager::AddTask(new ReadFromCGI(pid, fd[0], client_fd, port, ip, config));
  Logger::Info() << "ReadFromCGIを追加" << std::endl;
}


std::map<std::string, std::string> CgiHandler::GetEnv(HTTPRequest *req, const IConfig &config, const std::string &port, const std::string &ip) {
  std::map<std::string, std::string> env_map;
  (void)config;
  (void)ip;
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
  //uriからpath_infoを取得
  // env_map["PATH_INFO"] = req->GetUri();
  //TODO: 絶対パスの取得
  // env_map["PATH_TRANSLATED"] = req->GetUri();
  env_map["QUERY_STRING"] = req->GetUri().substr(req->GetUri().find("?") + 1);
  //TODO: 
  // env_map["REMOTE_ADDR"] = client_addr.sin_addr.s_addr;
  //REMOTE_HOSTは使用可能関数ではわからない
  env_map["REMOTE_HOST"] = "";
  //TODO: userの取得
  if (env_map["AUTH_TYPE"] == "")
    env_map["REMOTE_USER"] = "";
  else
    env_map["REMOTE_USER"] = "user";
  //TODO: スクリプト名の取得
  // env_map["SCRIPT_NAME"] = req->GetUri();
  //TODO: URIからサーバ名の取得
  env_map["SERVER_NAME"] = "localhost";
  env_map["SERVER_PORT"] = port;
  env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
  env_map["SERVER_SOFTWARE"] = "webserv";
  return env_map;
}

char **CgiHandler::DupEnv(const std::map<std::string, std::string> &env_map){
  char **env = new char *[env_map.size() + 1];
  Logger::Info() << "-------env_map--------" << env_map.size() << std::endl;
  std::map<std::string, std::string>::const_iterator it = env_map.begin();
  for (unsigned int i = 0; it != env_map.end(); ++it, ++i) {
    std::string tmp = it->first + "=" + it->second;
    env[i] = new char[tmp.size() + 1];
    if (env[i] == NULL){
      for (unsigned int j = 0; j < i; ++j)
        delete[] env[j];
      delete[] env;
      return NULL;
    }
    std::strcpy(env[i], tmp.c_str());
    Logger::Info() << env[i] << std::endl;
  }
  return env;
}
