#include "write_to_cgi.hpp"

WriteToCGI::WriteToCGI(int fd, const std::string &body)
    : AIOTask(fd, POLLOUT, kCGIFd), body_(body), wrote_size_(0) {}

WriteToCGI::~WriteToCGI() {}

Result<int, std::string> WriteToCGI::Execute(int revent) {
  if (!(event_ & revent)) return Ok(kNotReady);
  int ret = write(fd_, body_.c_str() + wrote_size_, body_.size() - wrote_size_);
  wrote_size_ += ret;
  if (wrote_size_ == body_.size()) {
    return Ok(kTaskDelete);
  }
  return Ok(kOk);
}
