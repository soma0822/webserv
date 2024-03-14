#ifndef WEBSERV_SRC_SERVER_READ_CGI_HPP
#define WEBSERV_SRC_SERVER_READ_CGI_HPP

#include <sys/wait.h>
#include <unistd.h>

#include "config.hpp"
#include "http_request.hpp"
#include "http_request_parser.hpp"
#include "http_response.hpp"
#include "io_task.hpp"
#include "io_task_manager.hpp"
#include "logger.hpp"
#include "request_handler.hpp"
#include "write_response_to_client.hpp"

class ReadCGI : public AIOTask {
 public:
  ReadCGI(int pid, int fd, int client_fd, const std::string &port,
          const std::string &ip, const IConfig &config);
  ~ReadCGI();
  Result<int, std::string> Execute();

  const std::string &GetPort() const;
  const std::string &GetIp() const;

 private:
  int client_fd_;
  const std::string port_;
  const std::string ip_;
  const IConfig &config_;
  HTTPRequestParser parser_;
  int event_;
  static const int buf_size_ = 1024;
  std::string buf_;
  int pid_;
};

#endif
