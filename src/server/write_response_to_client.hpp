#ifndef WEBSERV_SRC_WRITE_RESPONSE_TO_CLIENT_HPP
#define WEBSERV_SRC_WRITE_RESPONSE_TO_CLIENT_HPP

#include <poll.h>
#include <unistd.h>

#include <string>

#include "io_task.hpp"
#include "result.hpp"
#include "http_response.hpp"

class WriteResponseToClient : public IOTask {
public:
  WriteResponseToClient(int fd, const HTTPResponse *response);
  virtual ~WriteResponseToClient();
  virtual Result<int, std::string> Excecute();

private:
  HTTPResponse response_;
}

#endif // WEBSERV_SRC_WRITE_RESPONSE_TO_CLIENT_HPP
