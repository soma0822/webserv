#include "read_request_from_client.hpp"

ReadRequestFromClient::ReadRequestFromClient(int fd, const std::string &port,
                                             const std::string &ip,
                                             const IConfig &config)
    : AIOTask(fd, POLLIN),
      port_(port),
      ip_(ip),
      config_(config),
      parser_(HTTPRequestParser()) {}

ReadRequestFromClient::~ReadRequestFromClient() {}

Result<int, std::string> ReadRequestFromClient::Execute() {
  char buf[buf_size_];
  int len = read(fd_, buf, buf_size_);
  if (len == -1) {
    Logger::Error() << "read エラー" << std::endl;
    return Err("read error");
  }
  if (len == 0) return Ok(kReadDelete);
  buf[len] = '\0';
  Result<HTTPRequest *, int> result = parser_.Parser(buf);
  if (result.IsErr() && result.UnwrapErr() == HTTPRequestParser::kNotEnough) {
    return Ok(0);
    Logger::Info() << port_ << " : "
                   << "リクエストをパース中です : " << buf << len << std::endl;
  } else if (result.IsErr() &&
             result.UnwrapErr() == HTTPRequestParser::kBadRequest) {
    std::cout << "bad request" << std::endl;
    // TODO: badrequestの処理
     IOTaskManager::AddTask(new WriteResponseToClient(fd_,
     CreateBadRequest()));
  } else {
    HTTPResponse *response =
        RequestHandler::Handle(config_, result.Unwrap(), port_, ip_);
    IOTaskManager::AddTask(new WriteResponseToClient(fd_, response));
    Logger::Info() << port_ << " : "
                   << "リクエストをパースしました : " << buf << len
                   << std::endl;
    delete result.Unwrap();
  }
  Logger::Info() << port_ << " : "
                 << "レスポンスのタスクを追加しました" << std::endl;
  return Ok(0);
}

HTTPResponse *ReadRequestFromClient::CreateBadRequest(){
  HTTPResponse *bad_res = new HTTPResponse;
  bad_res->SetHTTPVersion("HTTP/1.1");
  bad_res->SetStatusCode(http::kBadRequest);
  bad_res->SetBody("<html><head><title>400 Bad Request</title></head><body><center><h1>400 Bad Request</h1></center><hr><center>nginx/1.25.3</center></body></html>");
  bad_res->AddHeader("Content-Length", "157");
  return bad_res;
}

const std::string &ReadRequestFromClient::GetPort() const { return port_; }
const std::string &ReadRequestFromClient::GetIp() const { return ip_; }
