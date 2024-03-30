#ifndef WEBSERV_SRC_SERVER_SERVER_HPP
#define WEBSERV_SRC_SERVER_SERVER_HPP

#include <fcntl.h>
#include <sys/socket.h>

#include <map>
#include <string>
#include <vector>

#include "accept.hpp"
#include "config.hpp"
#include "io_task_manager.hpp"
#include "logger.hpp"
#include "result.hpp"
#include "addr_utils.hpp"

class Server {
 public:
  static bool Run(const IConfig &config);

 private:
  Server();
  ~Server();
  Server(const Server &other);
  Server &operator=(const Server &other);
  static Result<int, int> Listen(const std::string &port,
                                 const std::string &ip);

  // TODO : errornoによって定義していく
  enum Error {
    kSocketError,
    kSetSockOptError,
    kListenError,
    kBindError,
    kFcntlError,
    kBadIP
  };
};

#endif
