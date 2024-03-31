#include "write_response_to_client.hpp"

WriteResponseToClient::WriteResponseToClient(int fd, HTTPResponse *response,
                                             HTTPRequest *request)
    : AIOTask(fd, POLLOUT, kClientSocket),
      response_(response),
      request_(request),
      wrote_size_(0) {}

WriteResponseToClient::~WriteResponseToClient() {
  delete request_;
  delete response_;
}

Result<int, std::string> WriteResponseToClient::Execute(int revent) {
  if (!(event_ & revent)) return Ok(kNotReady);
  std::string response_str = response_->ToString();
  int bytes_written = write(fd_, response_str.c_str() + wrote_size_,
                            response_str.size() - wrote_size_);
  if (bytes_written == -1 || bytes_written == 0) {
    Logger::Error() << "write エラー" << std::endl;
    return Ok(kFdDelete);
  }
  if ((wrote_size_ += bytes_written) == response_str.size()) {
    Logger::Info() << "レスポンスを書き込みました\n\n"
                   << response_str << std::endl;
    std::map<std::string, std::string>::const_iterator it_conn =
        request_->GetHeaders().find("CONNECTION");
    bool have_conn_close =
        (it_conn == request_->GetHeaders().end() ? false
                                                 : it_conn->second == "close");
    if (response_->GetStatusCode() == http::kBadRequest || have_conn_close)
      return Ok(kFdDelete);
    return Ok(kTaskDelete);
  }
  return Ok(kContinue);
}
