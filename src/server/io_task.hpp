#ifndef WEBSERV_SRC_SERVER_IO_TASK_HPP
#define WEBSERV_SRC_SERVER_IO_TASK_HPP

#include <string>

#include "result.hpp"

class AIOTask {
 public:
  AIOTask();
  virtual ~AIOTask();
  virtual Result<int, std::string> Execute() = 0;
  int GetFd() const;
  int GetEvent() const;

 protected:
  int fd_;
  int event_;
};

#endif
