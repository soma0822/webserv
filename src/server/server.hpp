#ifndef WEBSERV_SRC_SERVER_SERVER_HPP
#define WEBSERV_SRC_SERVER_SERVER_HPP

#include <map>
#include <string>
#include <vector>

#include "accept.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "result.hpp"
// #include "io_task_manager.hpp"

class Server {
 public:
  static void run();

 private:
  Server();
  ~Server();
  Server(const Server &other);
  Server &operator=(const Server &other);
  static Result<int, int> Listen(const std::string &port);

  // TODO : errornoによって定義していく
  enum error { klistenError, kBindError };
};

#endif
