#include "write_response_to_client.hpp"

WriteResponseToClient::WriteResponseToClient(int fd, HTTPResponse *response)
    : response_(response) {
  fd_ = fd;
  event_ = POLLOUT;
}

WriteResponseToClient::~WriteResponseToClient() { delete response_; }

Result<int, std::string> WriteResponseToClient::Execute() {
  std::string response_str = response_->ToString();
  unsigned int bytes_written = write(fd_, response_str.c_str(), response_str.size());
  if (bytes_written < 0) {
    return Err("ライトに失敗しました");
  }
  if ((writed_ += bytes_written) == response_str.size())
    return Ok(1);
  return Ok(0);
}
