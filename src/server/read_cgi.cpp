#include "read_cgi.hpp"

ReadCGI::ReadCGI(int pid, int fd, int client_fd, const std::string &port,
                 const std::string &ip, const IConfig &config)
    : AIOTask(fd, POLLIN),
      client_fd_(client_fd),
      port_(port),
      ip_(ip),
      config_(config),
      parser_(HTTPRequestParser()),
      pid_(pid) {}

ReadCGI::~ReadCGI() {}

Result<int, std::string> ReadCGI::Execute() {
  char buf[buf_size_ + 1];
  int status;
  int len = read(fd_, buf, buf_size_);
  buf[len] = '\0';
  buf_.append(buf);
  int result = waitpid(pid_, &status, WNOHANG);
  if (result == -1) {  // エラー
    Logger::Error() << "waitpid エラー: " << pid_ << std::endl;
    return Ok(kReadDelete);
  } else if (result == 0) {  // まだ終了していない
    return Ok(0);
  }
  if (WIFEXITED(status) == 0) {  // 異常終了
    Logger::Error() << "cgi エラー" << std::endl;
    return Ok(kReadDelete);
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
    return Ok(kReadDelete);
  }
  return Ok(0);
}
