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
                        struct sockaddr_in client_addr, const IConfig &config);
  virtual ~ReadRequestFromClient();
  virtual Result<int, std::string> Execute(int revent);
  Result<int, std::string> ExecuteReady(const std::string &buf);
  virtual Result<int, std::string> ExecuteNotReady();
  Result<int, std::string> AddErrResponse(http::StatusCode status);
  Result<int, std::string> AddResponse(HTTPRequest *request);
  const std::string &GetPort() const;
  const std::string &GetIp() const;

 private:
  ReadRequestFromClient();
  ReadRequestFromClient(const ReadRequestFromClient &other);
  ReadRequestFromClient &operator=(const ReadRequestFromClient &other);
  Result<std::string, AIOTask::IOTaskStatus> ReadRequest();
  std::string port_;
  std::string ip_;
  struct sockaddr_in client_addr_;
  const IConfig &config_;
  HTTPRequestParser parser_;
  static const int buf_size_ = 1024;
  enum Responce { kOk, kContinue, kBadRequest };
};

#endif
