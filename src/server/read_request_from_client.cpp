#include "read_request_from_client.hpp"

ReadRequestFromClient::ReadRequestFromClient(int fd, const std::string &port,
                                             const std::string &ip,
                                             struct sockaddr_in client_addr,
                                             const IConfig &config)
    : AIOTask(fd, POLLIN),
      port_(port),
      ip_(ip),
      client_addr_(client_addr),
      config_(config),
      parser_(HTTPRequestParser()) {}

ReadRequestFromClient::~ReadRequestFromClient() {Logger::Info() << ip_ << ":" << port_ << " : 接続を切りました" << std::endl;}

Result<int, std::string> ReadRequestFromClient::Execute() {
  char buf[buf_size_ + 1];
  int len = read(fd_, buf, buf_size_);
  if (len == -1) {
    Logger::Error() << "read エラー" << std::endl;
    return Ok(kFdDelete);
  }
  if (len == 0) {Logger::Info() << ip_ << ":" << port_ << " : 接続が切られました" << std::endl; return Ok(kFdDelete);}
  buf[len] = '\0';
  Result<HTTPRequest *, int> result = parser_.Parser(buf);
  if (result.IsErr() && result.UnwrapErr() == HTTPRequestParser::kNotEnough) {
    return Ok(kOk);
  } else if (result.IsErr() &&
             http::GetStatusMessage(
                 static_cast<http::StatusCode>(result.UnwrapErr()))
                     .empty() == false) {
    IOTaskManager::AddTask(new WriteResponseToClient(
        fd_,
        GenerateErrorResponse(static_cast<http::StatusCode>(result.UnwrapErr()),
                              config_),
        static_cast<HTTPRequest *>(NULL)));
  } else if (result.IsOk()) {
    Logger::Info() << "リクエストを受け取りました\n\n" << *result.Unwrap() << std::endl;
    RequestContext req_ctx = {result.Unwrap(), port_, ip_,
                              client_addr_,    fd_,   0};
    Option<HTTPResponse *> option = RequestHandler::Handle(config_, req_ctx);
    if (option.IsSome()) {
      IOTaskManager::AddTask(
          new WriteResponseToClient(fd_, option.Unwrap(), result.Unwrap()));
    }
  }
  return Ok(kOk);
}

const std::string &ReadRequestFromClient::GetPort() const { return port_; }
const std::string &ReadRequestFromClient::GetIp() const { return ip_; }
