#ifndef WEBSERV_SRC_SERVER_IO_TASK_HPP
#define WEBSERV_SRC_SERVER_IO_TASK_HPP

#include <string>

#include "result.hpp"

class AIOTask {
 public:
  enum IOTaskStatus { kOk, kContinue, kNotReady, kTaskDelete, kFdDelete };
  AIOTask();
  AIOTask(int fd, int event);
  virtual ~AIOTask();
  virtual Result<int, std::string> Execute(int revent) = 0;
  int GetFd() const;
  int GetEvent() const;

 protected:
  int fd_;
  int event_;
};

#endif
