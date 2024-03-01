#ifndef WEBSERV_SRC_SERVER_HPP
#define WEBSERV_SRC_SERVER_HPP

// struct sockaddr_in
#include <arpa/inet.h>
//socket
#include <sys/socket.h>

#include "io_task.hpp"
#include "logger.hpp"
#include "result.hpp"
#include "string_utils.hpp"

class Accept : public AIOTask {
public:
  Accept(const std::string &);
  virtual ~Accept();
  virtual Result<int, std::string> Execute();

private:
  Accept();
  Accept(const Accept &);
  Accept &operator=(const Accept &);
};


#endif