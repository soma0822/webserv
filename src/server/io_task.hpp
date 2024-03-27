#ifndef WEBSERV_SRC_SERVER_IO_TASK_HPP
#define WEBSERV_SRC_SERVER_IO_TASK_HPP

#include <string>

#include "result.hpp"
class AIOTask {
 public:
  enum IOTaskStatus { kOk, kContinue, kNotReady, kTaskDelete, kFdDelete };
  AIOTask();
  AIOTask(int fd, int event, int timeout_s);
  virtual ~AIOTask();
  virtual Result<int, std::string> Execute(int revent) = 0;
  int GetFd() const;
  int GetEvent() const;
  int GetTimeout() const;

 protected:
  int fd_;
  int event_;
  int timeout_s_;

  enum Timeout {
    kAccept = 0,
    kClientSocket = 60,
    kCGIFd = 10
  };
};

#endif
