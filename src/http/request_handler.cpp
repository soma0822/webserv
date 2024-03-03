#include "request_handler.hpp"

const HTTPResponse *RequestHandler::Handle(const HTTPRequest *request,
                                           const std::string &port) {
  (void)request;
  (void)port;
  return new HTTPResponse();
}

RequestHandler::RequestHandler() {}

RequestHandler::RequestHandler(const RequestHandler &other) { (void)other; }

RequestHandler &RequestHandler::operator=(const RequestHandler &other) {
  (void)other;
  return *this;
}

RequestHandler::~RequestHandler() {}