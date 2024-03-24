#ifndef WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_
#define WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_

#include "config.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "io_task_manager.hpp"
#include "option.hpp"
#include "read_from_cgi.hpp"
#include "write_to_cgi.hpp"

class RequestHandler {
 public:
  static Option<HTTPResponse *> Handle(const IConfig &config,
                                       RequestContext req_ctx);
  static Option<HTTPResponse *> Get(const IConfig &config,
                                    RequestContext req_ctx);
  static Option<HTTPResponse *> Post(const IConfig &config,
                                     RequestContext req_ctx);
  static Option<HTTPResponse *> Delete(const IConfig &config,
                                       RequestContext req_ctx);
  static http::StatusCode CGIExe(const IConfig &config, RequestContext req_ctx,
                                 const std::string &program_path,
                                 const std::string &script_name);

 private:
  RequestHandler();
  RequestHandler(const RequestHandler &other);
  RequestHandler &operator=(const RequestHandler &other);
  ~RequestHandler();

  static const int kMaxUriLength = 2048;

  static HTTPResponse *GenerateAutoIndexPage(
      const IConfig &config, const HTTPRequest *request,
      const std::string &abs_path);
  static std::map<std::string, std::string> GetEnv(
      const IConfig &config, const RequestContext &req_ctx);
  static char **DupEnv(const std::map<std::string, std::string> &env_map);
  static void DeleteEnv(char **env);
};

#endif  // WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_
