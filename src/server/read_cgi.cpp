#include "read_cgi.hpp"

ReadCGI::ReadCGI(int pid, int fd, int client_fd, const std::string &port,
                 const std::string &ip, const IConfig &config)
    : pid_(pid),
      AIOTask(fd, POLLIN),
      client_fd_(client_fd),
      port_(port),
      ip_(ip),
      config_(config),
      parser_(HTTPRequestParser()) {}

ReadCGI::~ReadCGI() {}

Result<int, std::string> ReadCGI::Execute() {
  char buf[buf_size_];
  int status;
  int result = waitpid(pid_, &status, WNOHANG);
  if (result == -1) {  // エラー
    Logger::Error() << "waitpid エラー: " << pid_ << std::endl;
    return Ok(kReadDelete);
  } else
    (result == 0) {  // まだ終了していない
      return Ok(0);
    }
  if (WIFEXITED(status) == 0) {  // 異常終了
    Logger::Error() << "cgi エラー" << std::endl;
    return Ok(kReadDelete);
  }
  int len = read(fd_, buf, buf_size_);
  buf[len] = '\0';
  if (len == 0 && buf_.size() == 0) return Ok(0);
  if (len == buf_size_) {
    buf_.append(buf);
    return Ok(0);
  }
  buf_.append(buf);
  // Result<HTTPRequest *, int> result = parser_.Parser(buf);
  HTTPResponse *response = new HTTPResponse();
  response->SetHTTPVersion("HTTP/1.1");
  response->SetStatusCode(http::kOk);
  response->AddHeader("Content-Type", "text/html");
  response->AddHeader("Content-Length", std::to_string(buf_.size()));
  response->SetBody(buf_);
  Logger::Info() << "CGI response: " << buf_ << "\nsize: " << len << std::endl;
  //   RequestHandler::Handle(config_, result.Unwrap(), port_, ip_);
  IOTaskManager::AddTask(new WriteResponseToClient(client_fd_, response));
  return Ok(kReadDelete);
}
