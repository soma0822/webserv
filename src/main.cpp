#include <iostream>

#include "config.hpp"
#include "config_parser.hpp"
#include "http_request_parser.hpp"
#include "logger.hpp"
#include "server.hpp"

// int main(int ac, char **av) {
//   Logger::SetHandler(new FileStreamWrapper("log.txt"));
//   std::string config_file;
//   if (ac == 1)
//     config_file = ConfigParser::default_file_;
//   else if (ac == 2)
//     config_file = av[1];
//   else {
//     Logger::Error() << "Invalid argument." << std::endl;
//     return 0;
//   }
//   Logger::Info() << "Reading " << config_file << std::endl;
//   try {
//     const Config config = ConfigParser::Parse(config_file);
//     const std::map<std::string, std::string> &v = config.GetErrorPage();
//     for (std::map<std::string, std::string>::const_iterator it = v.begin();
//          it != v.end(); ++it) {
//       Logger::Info() << it->first << " : " << it->second << std::endl;
//     }
//     const std::vector<ServerContext> &m = config.GetServer();
//     for (std::vector<ServerContext>::const_iterator it = m.begin();
//          it != m.end(); ++it) {
//       Logger::Info() << *it << std::endl;
//     }
//     Server::Run(config);
//   } catch (std::exception &e) {
//     std::cerr << e.what() << std::endl;
//   }
// }
#include "cgi_handler.hpp"
#include "cgi_parser.hpp"
#include "file_utils.hpp"
int main() {
  Config config;
  CGIParser parser;
  HTTPRequest* req =
      parser
          .Parser(
              "Status: 200\r\nContent-Type: text/html\r\ncharset: "
              "UTF-8\r\n\r\n<html>\r\n    <head>\r\n        "
              "<title>サーバーテスト</title>\r\n        <meta "
              "http-equiv=\"content-type\" charset=\"utf-8\">\r\n    "
              "</head>\r\n    <body>\r\n    <form "
              "action=\"/cgi-bin/cgi_test.py\" method=\"POST\">\r\n        "
              "<div>\r\n            <label "
              "for=\"name\">好きな食べ物</label>\r\n            <input "
              "type=\"text\" name=\"food\" value=\"りんご\">\r\n            "
              "<label for=\"season\">好きな季節</label>\r\n            <input "
              "type=\"text\" name=\"season\" value=\"冬\">\r\n            "
              "<button>送信</button>\r\n        </div>\r\n    </form>\r\n    "
              "</body>\r\n</html>\r\n")
          .Unwrap();
  RequestContext req_ctx;
  std::cout << CGIHandler::Handle(config, req, req_ctx).Unwrap()->ToString()
            << std::endl;
}
