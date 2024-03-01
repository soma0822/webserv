#include "request_handler.hpp"

const HTTPResponse *RequestHandler::Handle(const HTTPRequest *request) {
  (void)request;
  return new HTTPResponse();
}

RequestHandler::RequestHandler() {}

RequestHandler::RequestHandler(const RequestHandler &other) { (void)other; }

RequestHandler &RequestHandler::operator=(const RequestHandler &other) {
  (void)other;
  return *this;
}

RequestHandler::~RequestHandler() {}