#ifndef WEBSERV_SRC_SERVER_READ_REQUEST_FROM_CLIENT_HPP
#define WEBSERV_SRC_SERVER_READ_REQUEST_FROM_CLIENT_HPP
#include <poll.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "http_response.hpp"
// #include "http_request.hpp"
#include "io_task.hpp"
#include "logger.hpp"
#include "request_handler.hpp"
#include "result.hpp"

class ReadRequestFromClient : public AIOTask {
 public:
  ReadRequestFromClient(int fd, const std::string &port);
  virtual ~ReadRequestFromClient();
  virtual Result<int, std::string> Execute();
  const std::string &GetPort() const;

 private:
  ReadRequestFromClient();
  ReadRequestFromClient(const ReadRequestFromClient &other);
  ReadRequestFromClient &operator=(const ReadRequestFromClient &other);
  std::string port_;
  // RequestParser parser_;
};

#endif
