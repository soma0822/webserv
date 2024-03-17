#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <unistd.h>

#include <iostream>

#include "http_request.hpp"
#include "http_response.hpp"
#include "io_task.hpp"
#include "io_task_manager.hpp"
#include "read_cgi.hpp"

class CgiHandler {
 public:
  CgiHandler() {}
  ~CgiHandler() {}

  void run() {
    std::cout << "Content-type: text/html" << std::endl << std::endl;
    std::cout << "<html><head><title>CGI Test</title></head><body><h1>CGI "
                 "Test</h1></body></html>"
              << std::endl;
  }
  void Handle(HTTPRequest *req, int client_fd, const IConfig &config_,
              const std::string &port, const std::string &ip);
};

#endif
