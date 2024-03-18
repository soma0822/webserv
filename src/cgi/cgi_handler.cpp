#include "cgi_handler.hpp"

void CgiHandler::Handle(HTTPRequest *req, int client_fd, const IConfig &config,
                        const std::string &port, const std::string &ip) {
  std::string body = req->GetBody();
  int fd[2];
  int fd2[2];
  pid_t pid;
  if (pipe(fd) == -1) {
    Logger::Error() << "pipe エラー" << std::endl;
    return;
  }
  if (fcntl(fd[0], F_SETFL, O_NONBLOCK) == -1) {
    Logger::Error() << "fcntl エラー" << std::endl;
    return;
  }
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
  pid = fork();
  if (pid == -1) {
    Logger::Error() << "fork エラー" << std::endl;
    return;
  }
  if (pid == 0) {
    close(fd[0]);
    close(fd2[1]);
    dup2(fd[1], 1);
    dup2(fd2[0], 0);
    std::stringstream ss;
    ss << "Content-length=" << body.size();
    std::string content_length = ss.str();
    char cwd[1024];
    getcwd(cwd, 1024);
    std::string cgi_file = cwd + std::string("/cgi-bin/default.py");
    const char *ex[] = {"REQUEST_METHOD=POST", content_length.c_str(), NULL};
    const char *argv[] = {"/usr/bin/python3",
                          cgi_file.c_str(),
                          NULL};
    Logger::Info() << "CGI実行" << std::endl;
    execve("/usr/bin/python3", const_cast<char *const *>(argv),
           const_cast<char *const *>(ex));
  }
  close(fd[1]);
  IOTaskManager::AddTask(new ReadFromCGI(pid, fd[0], client_fd, port, ip, config));
  Logger::Info() << "ReadFromCGIを追加" << std::endl;
}
