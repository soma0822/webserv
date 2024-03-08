#include "read_request_from_client.hpp"

ReadRequestFromClient::ReadRequestFromClient(int fd, const std::string &port,
                                             const std::string &ip,
                                             const IConfig &config)
    : AIOTask(fd, POLLIN), port_(port), ip_(ip), config_(config) {
  // parser_ = RequestParser();
}

ReadRequestFromClient::~ReadRequestFromClient() {}

Result<int, std::string> ReadRequestFromClient::Execute() {
  char buf[1024];
  int len = read(fd_, buf, sizeof(buf));
  if (len == -1) {
    Logger::Error() << "read エラー" << std::endl;
    return Err("read error");
  }
  buf[len] = '\0';
  // 　参考程度
  //  Result<HTTPRequest, int> result = parser_.Parse(buf.c_str());
  //  Logger::Info() << port_ << " : " << "リクエストをパースしました" <<
  //  std::endl; if (result.IsErr() && result.UnwrapErr() == CONTINUE) {
  //    return Ok(0);
  //  } else if (result.IsErr() && result.UnwrapErr() == BAD_REQUEST) {
  //    IOTaskManager::AddTask(new WriteResponseToClient write_response(fd_,
  //    badrequest_response));
  //  } else {
  //    HTTPResponse *response = RequestHandler::Handle(result.Unwrap(), port_,
  //    ip_); IOTaskManager::AddTask(new WriteResponseToClient(fd_, response));
  //  }
  Logger::Info() << port_ << " : "
                 << "レスポンスのタスクを追加しました" << std::endl;
  return Ok(0);
}

const std::string &ReadRequestFromClient::GetPort() const { return port_; }
const std::string &ReadRequestFromClient::GetIp() const { return ip_; }
