#ifndef WEBSERV_SRC_WRITE_RESPONSE_TO_CLIENT_HPP
#define WEBSERV_SRC_WRITE_RESPONSE_TO_CLIENT_HPP

#include <poll.h>
#include <unistd.h>

#include <string>

#include "http_response.hpp"
#include "io_task.hpp"
#include "logger.hpp"
#include "result.hpp"
#include "http_request.hpp"

class WriteResponseToClient : public AIOTask {
 public:
  WriteResponseToClient(int fd, HTTPResponse *response, HTTPRequest *request);
  virtual ~WriteResponseToClient();
  virtual Result<int, std::string> Execute();

 private:
  WriteResponseToClient();
  WriteResponseToClient(const WriteResponseToClient &other);
  HTTPResponse *response_;
  HTTPRequest *request_;
  unsigned int wrote_size_;
};

#endif  // WEBSERV_SRC_WRITE_RESPONSE_TO_CLIENT_HPP
