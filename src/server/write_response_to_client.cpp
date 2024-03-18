#include "write_response_to_client.hpp"

WriteResponseToClient::WriteResponseToClient(int fd, HTTPResponse *response)
    : response_(response) {
  fd_ = fd;
  event_ = POLLOUT;
}

WriteResponseToClient::~WriteResponseToClient() { delete response_; }

Result<int, std::string> WriteResponseToClient::Execute() {
  std::string response_str = response_->ToString();
  unsigned int bytes_written =
      write(fd_, response_str.c_str(), response_str.size());
  if ((wrote_size_ += bytes_written) == response_str.size())
    return Ok(kTaskDelete);
  return Ok(0);
}
