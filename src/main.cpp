#include <iostream>

#include "config.hpp"
#include "config_parser.hpp"
#include "http_request_parser.hpp"
#include "logger.hpp"

void CheckRequest(std::string request, HTTPRequestParser &parser) {
  Result<HTTPRequest *, int> req = parser.Parser(request);
  if (req.IsErr()) {
    std::cerr << "Error: " << req.UnwrapErr() << std::endl;
  } else {
    std::cout << "Method: " << req.Unwrap()->GetMethod() << std::endl;
    std::cout << "URI: " << req.Unwrap()->GetUri() << std::endl;
    std::cout << "Protocol: " << req.Unwrap()->GetProtocol() << std::endl;
    std::cout << "Version: " << req.Unwrap()->GetVersion() << std::endl;
    std::cout << "Host: " << req.Unwrap()->GetHostHeader() << std::endl;
    std::cout << "Body: " << req.Unwrap()->GetBody() << std::endl;
  }
}

int main() {
  HTTPRequestParser parser;
  std::string request =
      "POST / HTTP/1.1\r\nHost: localhost:8080\r\nTransfer-Encoding: "
      "chunked\r\n\r\n\r\n";
  CheckRequest(request, parser);
  // CheckRequest("5\r\n", parser);
  // CheckRequest("hello\r\n", parser);
  // CheckRequest("\r\n", parser);
  // CheckRequest("0\r\n", parser);
  // CheckRequest("\r\n", parser);

  // std::string config_file;
  // if (ac == 1)
  //   config_file = ConfigParser::default_file_;
  // else if (ac == 2)
  //   config_file = av[1];
  // else {
  //   Logger::Error() << "Invalid argument." << std::endl;
  //   return 0;
  // }
  // Logger::Info() << "Reading " << config_file << std::endl;
  // try {
  //   Config config = ConfigParser::Parse(config_file);
  //   std::vector<ServerContext> m = config.GetServer();
  //   for (std::vector<ServerContext>::const_iterator it = m.begin();
  //        it != m.end(); ++it) {
  //     std::cout << *it << std::endl;
  //   }
  // } catch (std::exception &e) {
  //   std::cerr << e.what() << std::endl;
  // }
  // return 0;
}
