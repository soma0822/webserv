#include "read_request_from_client.hpp"

ReadRequestFromClient::ReadRequestFromClient(int fd, const std::string &port,
                                             const std::string &ip,
                                             struct sockaddr_in client_addr,
                                             const IConfig &config)
    : AIOTask(fd, POLLIN, kClientSocket),
      port_(port),
      ip_(ip),
      client_addr_(client_addr),
      config_(config),
      parser_(HTTPRequestParser()) {}

ReadRequestFromClient::~ReadRequestFromClient() {
  Logger::Info() << ip_ << ":" << port_ << " : 接続を切りました" << std::endl;
}

Result<int, std::string> ReadRequestFromClient::Execute(int revent) {
  if (!(event_ & revent)) {
    return ExecuteNotReady();
  }
  Result<std::string, AIOTask::IOTaskStatus> read_result = ReadRequest();
  if (read_result.IsErr()) return Ok(read_result.UnwrapErr());
  return ExecuteReady(read_result.Unwrap());
}

Result<int, std::string> ReadRequestFromClient::ExecuteReady(
    const std::string &buf) {
  Result<HTTPRequest *, int> result = parser_.Parser(buf);
  if (result.IsErr() &&
      http::GetStatusMessage(static_cast<http::StatusCode>(result.UnwrapErr()))
              .empty() == false) {
    return AddErrResponse(static_cast<http::StatusCode>(result.UnwrapErr()));
  } else if (result.IsOk()) {
    return AddResponse(result.Unwrap());
  }
  return Ok(kOk);
}

Result<int, std::string> ReadRequestFromClient::ExecuteNotReady() {
  Result<HTTPRequest *, int> result = parser_.Parser("");
  if (result.IsErr() &&
      http::GetStatusMessage(static_cast<http::StatusCode>(result.UnwrapErr()))
              .empty() == false) {
    return AddErrResponse(static_cast<http::StatusCode>(result.UnwrapErr()));
  } else if (result.IsOk()) {
    return AddResponse(result.Unwrap());
  }
  return Ok(kNotReady);
}

Result<int, std::string> ReadRequestFromClient::AddErrResponse(
    http::StatusCode status) {
  IOTaskManager::AddTask(
      new WriteResponseToClient(fd_, GenerateErrorResponse(status, config_),
                                static_cast<HTTPRequest *>(NULL)));
  return Ok(kOk);
}

Result<int, std::string> ReadRequestFromClient::AddResponse(
    HTTPRequest *request) {
  Logger::Info() << "リクエストを受け取りました\n\n" << *request << std::endl;
  RequestContext req_ctx = {request, port_, ip_, client_addr_, fd_, 0};
  Option<HTTPResponse *> option = RequestHandler::Handle(config_, req_ctx);
  if (option.IsSome()) {
    IOTaskManager::AddTask(
        new WriteResponseToClient(fd_, option.Unwrap(), request));
  }
  return Ok(kOk);
}

Result<std::string, AIOTask::IOTaskStatus>
ReadRequestFromClient::ReadRequest() {
  char buf[buf_size_ + 1];
  int len = read(fd_, buf, buf_size_);
  if (len == -1) {
    Logger::Error() << "read エラー" << std::endl;
    return Err(kFdDelete);
  }
  if (len == 0) {
    Logger::Info() << ip_ << ":" << port_ << " : 接続が切られました"
                   << std::endl;
    return Err(kFdDelete);
  }
  buf[len] = '\0';
  return Ok(buf);
}

const std::string &ReadRequestFromClient::GetPort() const { return port_; }
const std::string &ReadRequestFromClient::GetIp() const { return ip_; }
