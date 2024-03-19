#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <unistd.h>

#include <iostream>

#include "http_request.hpp"
#include "http_response.hpp"
#include "io_task.hpp"
#include "io_task_manager.hpp"
#include "read_from_cgi.hpp"
#include "write_to_cgi.hpp"

class CgiHandler {
 public:
  CgiHandler() {}
  ~CgiHandler() {}

  void Handle(HTTPRequest *req, int client_fd, const IConfig &config_,
              const std::string &port, const std::string &ip);
  
  private:
    std::map<std::string, std::string> GetEnv(HTTPRequest *req, const IConfig &config,
                                             const std::string &port, const std::string &ip);
    char **DupEnv(const std::map<std::string, std::string> &env_map);
};

#endif
