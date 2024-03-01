#ifndef WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_
#define WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_

namespace http {

class RequestHandler {
 public:
  Handle(const HTTPRequest &request);

 private:
  RequestHandler();
  RequestHandler(const RequestHandler &other);
  RequestHandler &operator=(const RequestHandler &other);
  ~RequestHandler();
};

}  // namespace http

#endif  // WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_
