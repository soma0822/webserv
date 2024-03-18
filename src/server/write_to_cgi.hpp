#ifndef WEBSERV_SRC_SERVER_WRITE_TO_CGI_HPP
#define WEBSERV_SRC_SERVER_WRITE_TO_CGI_HPP

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

class WriteToCGI : public AIOTask {
 public:
  WriteToCGI(int fd, const std::string &body);
  ~WriteToCGI();
  Result<int, std::string> Execute();

 private:
  std::string body_;
  unsigned int writed_;
};

#endif
