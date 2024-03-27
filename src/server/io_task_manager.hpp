#ifndef WEBSERV_SRC_SERVER_IO_TASK_MANAGER_HPP
#define WEBSERV_SRC_SERVER_IO_TASK_MANAGER_HPP

#include <poll.h>

#include <iostream>
#include <vector>

#include "accept.hpp"
#include "io_task.hpp"
#include "time.hpp"
#include "read_request_from_client.hpp"
#include "write_response_to_client.hpp"

struct Tasks {
  std::vector<AIOTask *> tasks;
  unsigned int index;
  timespec ts;
};

class IOTaskManager {
 public:
  ~IOTaskManager();
  static void AddTask(AIOTask *task);
  static void RemoveFd(AIOTask *task);
  static void RemoveTask(AIOTask *task);
  static void DeleteTasks();
  static void ExecuteTasks();
  static const std::vector<Tasks> &GetTasks();
  static const std::vector<struct pollfd> &GetFds();

 private:
  IOTaskManager();
  IOTaskManager(const IOTaskManager &other);
  IOTaskManager &operator=(const IOTaskManager &other);
  static void CheckTimeout();
  static std::vector<Tasks> tasks_array_;
  static std::vector<struct pollfd> fds_;
  static const int poll_time_out_ = 5000;
};

#endif
