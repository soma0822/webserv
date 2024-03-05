#include "request_handler.hpp"
#include "config.hpp"

const HTTPResponse *RequestHandler::Handle(const HTTPRequest *request,
                                           const std::string &port) {
  (void)request;
  (void)port;
  return new HTTPResponse();
}

const HTTPResponse *RequestHandler::Get(const HTTPRequest *request,
                                         const std::string &port) {
  // TODO Get server via Config
  ServerContext server;
  const std::string &root_dir = server.GetRoot();
  const std::string &request_uri = request->GetUri();

  const std::string requested_file_path = root_dir + request_uri;

  return new HTTPResponse();
}

RequestHandler::RequestHandler() {}

RequestHandler::RequestHandler(const RequestHandler &other) { (void)other; }

RequestHandler &RequestHandler::operator=(const RequestHandler &other) {
  (void)other;
  return *this;
}

RequestHandler::~RequestHandler() {}