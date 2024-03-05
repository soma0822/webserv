#ifndef WEBSERV_SRC_WRITE_RESPONSE_TO_CLIENT_HPP
#define WEBSERV_SRC_WRITE_RESPONSE_TO_CLIENT_HPP

#include <poll.h>
#include <unistd.h>

#include <string>

#include "http_response.hpp"
#include "io_task.hpp"
#include "result.hpp"

class WriteResponseToClient : public AIOTask {
 public:
  WriteResponseToClient(int fd, HTTPResponse *response);
  virtual ~WriteResponseToClient();
  virtual Result<int, std::string> Excecute();

 private:
  const HTTPResponse *response_;
};

#endif  // WEBSERV_SRC_WRITE_RESPONSE_TO_CLIENT_HPP
