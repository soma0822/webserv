#include "request_handler.hpp"

int http::RequestHandler::Handle(const int &request) { return HTTPResponse(); }

http::RequestHandler::RequestHandler() {}

http::RequestHandler::RequestHandler(const RequestHandler &other) {
  (void)other;
}

http::RequestHandler &http::RequestHandler::operator=(
    const RequestHandler &other) {
  (void)other;
  return *this;
}

http::RequestHandler::~RequestHandler() {}