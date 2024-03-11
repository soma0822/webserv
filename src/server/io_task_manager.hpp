#ifndef WEBSERV_SRC_SERVER_IO_TASK_MANAGER_HPP
#define WEBSERV_SRC_SERVER_IO_TASK_MANAGER_HPP

#include <poll.h>

#include <iostream>
#include <vector>

#include "accept.hpp"
#include "io_task.hpp"
#include "read_request_from_client.hpp"
#include "write_response_to_client.hpp"

class IOTaskManager {
 public:
  ~IOTaskManager();
  static void AddTask(AIOTask *task);
  static void RemoveReadTask(AIOTask *task);
  static void RemoveWriteTask(AIOTask *task);
  static void ExecuteTasks();
  static const std::vector<std::vector<AIOTask *> > &GetTasks();
  static const std::vector<struct pollfd> &GetFds();

 private:
  IOTaskManager();
  IOTaskManager(const IOTaskManager &other);
  IOTaskManager &operator=(const IOTaskManager &other);
  static std::vector<std::vector<AIOTask *> > tasks_;
  static std::vector<struct pollfd> fds_;
  static const int poll_time_out_ = 5000;
  enum Error { kOk, kDelete };
};

#endif
