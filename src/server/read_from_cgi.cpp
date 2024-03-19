#include "read_from_cgi.hpp"

ReadFromCGI::ReadFromCGI(int pid, int fd, int client_fd,
                         const std::string &port, const std::string &ip,
                         const IConfig &config)
    : AIOTask(fd, POLLIN),
      client_fd_(client_fd),
      port_(port),
      ip_(ip),
      config_(config),
      parser_(HTTPRequestParser()),
      pid_(pid) {}

ReadFromCGI::~ReadFromCGI() {}

Result<int, std::string> ReadFromCGI::Execute() {
  char buf[buf_size_ + 1];
  int status;
  int len = read(fd_, buf, buf_size_);
  if (len == -1) {
    Logger::Error() << "read エラー" << std::endl;
    return Ok(kFdDelete);
  }
  buf[len] = '\0';
  buf_.append(buf);
  int result = waitpid(pid_, &status, WNOHANG);
  if (result == -1) {  // エラー
    Logger::Error() << "waitpid エラー: " << pid_ << std::endl;
    return Ok(kFdDelete);
  } else if (result == 0) {  // まだ終了していない
    return Ok(0);
  }
  if (WIFEXITED(status) == 0) {  // 異常終了
    Logger::Error() << "cgi エラー" << std::endl;
    return Ok(kFdDelete);
  }
  if (len == 0) {
    // Result<HTTPRequest *, int> result = parser_.Parser(buf);
    HTTPResponse *response = new HTTPResponse();
    response->SetHTTPVersion("HTTP/1.1");
    response->SetStatusCode(http::kOk);
    response->AddHeader("Content-Type", "text/html");
    std::stringstream ss;
    ss << buf_.size();
    response->AddHeader("Content-Length", ss.str());
    response->SetBody(buf_);
    Logger::Info() << "CGI response: " << buf_ << "\nsize: " << len
                   << std::endl;
    //   RequestHandler::Handle(config_, result.Unwrap(), port_, ip_);
    IOTaskManager::AddTask(new WriteResponseToClient(client_fd_, response));
    return Ok(kFdDelete);
  }
  return Ok(0);
}
