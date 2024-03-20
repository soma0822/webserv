#ifndef WEBSERV_SRC_SERVER_READ_FROM_CGI_HPP
#define WEBSERV_SRC_SERVER_READ_FROM_CGI_HPP

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

class ReadFromCGI : public AIOTask {
 public:
  ReadFromCGI(int pid, int fd, int client_fd, RequestContext req_ctx,
              const IConfig &config);
  ~ReadFromCGI();
  Result<int, std::string> Execute();

 private:
  int client_fd_;
  RequestContext req_ctx_;
  const IConfig &config_;
  HTTPRequestParser parser_;
  static const int buf_size_ = 1024;
  std::string buf_;
  int pid_;
};

#endif
