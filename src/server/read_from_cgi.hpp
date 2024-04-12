#ifndef WEBSERV_SRC_SERVER_READ_FROM_CGI_HPP
#define WEBSERV_SRC_SERVER_READ_FROM_CGI_HPP

#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include "cgi_handler.hpp"
#include "cgi_parser.hpp"
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
  ReadFromCGI(int pid, int fd, RequestContext req_ctx, const IConfig &config,
              timespec ts);
  ~ReadFromCGI();
  Result<int, std::string> Execute(int revent);

 private:
  RequestContext req_ctx_;
  const IConfig &config_;
  CGIParser parser_;
  static const int buf_size_ = 1024;
  std::string buf_;
  int pid_;
  timespec ts_;
  bool sended_signal_;
  const static int child_process_timeout_ = 2;
};

#endif
