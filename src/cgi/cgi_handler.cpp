#include "cgi_handler.hpp"

void CgiHandler::Handle(HTTPRequest *req, int client_fd, const IConfig &config,
                        const std::string &port, const std::string &ip) {
  std::string body = req->GetBody();
  int fd[2];
  pid_t pid;
  if (pipe(fd) == -1) {
    Logger::Error() << "pipe エラー" << std::endl;
    return;
  }
  if (fcntl(fd[0], F_SETFL, O_NONBLOCK) == -1) {
    Logger::Error() << "fcntl エラー" << std::endl;
    return;
  }
  pid = fork();
  if (pid == -1) {
    Logger::Error() << "fork エラー" << std::endl;
    return;
  }
  if (pid == 0) {
    dup2(fd[1], 1);
    close(fd[0]);
    int fd2[2];
    if (pipe(fd2) == -1) {
      Logger::Error() << "pipe エラー" << std::endl;
      return;
    }
    write(fd2[1], body.c_str(), body.size());
    close(fd2[1]);
    dup2(fd2[0], 0);
    std::stringstream ss;
    ss << "Content-length=" << body.size();
    std::string content_length = ss.str();
    const char *ex[] = {"REQUEST_METHOD=POST", content_length.c_str(), NULL};
    const char *argv[] = {"/usr/bin/python3",
                          "/Users/soma/Desktop/kakun/cgi-bin/cgi_test.py",
                          NULL};
    execve("/usr/bin/python3", const_cast<char *const *>(argv),
           const_cast<char *const *>(ex));
  }
  close(fd[1]);
  IOTaskManager::AddTask(new ReadCGI(pid, fd[0], client_fd, port, ip, config));
}
