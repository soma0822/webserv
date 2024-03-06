#ifndef WEBSERV_SRC_SERVER_ACCEPT_HPP
#define WEBSERV_SRC_SERVER_ACCEPT_HPP

#include <poll.h>

#include <cstring>
// struct sockaddr_in
#include <arpa/inet.h>
// socket
#include <sys/socket.h>

#include "io_task.hpp"
#include "logger.hpp"
#include "result.hpp"
#include "string_utils.hpp"

class Accept : public AIOTask {
 public:
  Accept(int fd, const std::string &port, const std::string &ip);
  virtual ~Accept();
  virtual Result<int, std::string> Execute();
  const std::string &GetPort() const;
  const std::string &GetIp() const;

 private:
  Accept();
  Accept(const Accept &other);
  Accept &operator=(const Accept &other);
  std::string port_;
  std::string ip_;
};

#endif
