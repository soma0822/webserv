#ifndef WEBSERV_SRC_SERVER_IOTASKMANAGER_HPP
#define WEBSERV_SRC_SERVER_IOTASKMANAGER_HPP

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
  static void RemoveTask(AIOTask *task);
  static void ExecuteTasks();

 private:
  IOTaskManager();
  IOTaskManager(const IOTaskManager &);
  IOTaskManager &operator=(const IOTaskManager &);
  static std::vector<std::vector<AIOTask *> > tasks_;
  static std::vector<struct pollfd> fds_;

  enum Error { kOk, kDelete };
};

#endif
