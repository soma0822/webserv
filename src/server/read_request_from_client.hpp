#ifndef WEBSERV_SRC_SERVER_READ_REQUEST_FROM_CLIENT_HPP
#define WEBSERV_SRC_SERVER_READ_REQUEST_FROM_CLIENT_HPP
#include <poll.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "config.hpp"
#include "http_request.hpp"
#include "http_request_parser.hpp"
#include "http_response.hpp"
#include "io_task.hpp"
#include "io_task_manager.hpp"
#include "logger.hpp"
#include "request_handler.hpp"
#include "result.hpp"
#include "write_response_to_client.hpp"

class ReadRequestFromClient : public AIOTask {
 public:
  ReadRequestFromClient(int fd, const std::string &port, const std::string &ip,
                        const IConfig &config);
  virtual ~ReadRequestFromClient();
  virtual Result<int, std::string> Execute();
  const std::string &GetPort() const;
  const std::string &GetIp() const;

 private:
  ReadRequestFromClient();
  ReadRequestFromClient(const ReadRequestFromClient &other);
  ReadRequestFromClient &operator=(const ReadRequestFromClient &other);
  std::string port_;
  std::string ip_;
  const IConfig &config_;
  HTTPRequestParser parser_;
  enum Responce { kOk, kContinue, kBadRequest };
};

#endif
