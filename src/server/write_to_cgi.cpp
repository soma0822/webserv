#include "write_to_cgi.hpp"

WriteToCGI::WriteToCGI(int fd, const std::string &body)
    : AIOTask(fd, POLLOUT), body_(body), writed_(0) {}

WriteToCGI::~WriteToCGI() {}

Result<int, std::string> WriteToCGI::Execute() {
  int ret = write(fd_, body_.c_str() + writed_, body_.size() - writed_);
  writed_ += ret;
  if (writed_ == body_.size()) {
    return Ok(kFdDelete);
  }
  return Ok(kOk);
}
