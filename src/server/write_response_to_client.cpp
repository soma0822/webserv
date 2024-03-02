#include "write_response_to_client.hpp"

WriteResponseToClient::WriteResponseToClient(int fd, const HTTPResponse *response) {
  fd_ = fd;
  event_ = POLLOUT;
  response_ = response;
}

WriteResponseToClient::~WriteResponseToClient() {
  delete response_;
}

Result<int, std::string> WriteResponseToClient::Excecute() {
  std::string response_str = response_->ToString();
  int bytes_written = write(fd_, response_str, response_str.size());
  if (bytes_written < 0) {
    return Err("ライトに失敗しました");
  }
  return Ok(1);
}
