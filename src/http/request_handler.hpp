#ifndef WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_
#define WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_

#include "config.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "io_task_manager.hpp"

class RequestHandler {
 public:
  static HTTPResponse *Handle(const IConfig &config, const HTTPRequest *request,
                              const std::string &port, const std::string &ip);
  static HTTPResponse *Get(const IConfig &config, const HTTPRequest *request,
                           const std::string &port, const std::string &ip);
  static HTTPResponse *Post(const IConfig &config, const HTTPRequest *request,
                            const std::string &port, const std::string &ip);
  static HTTPResponse *Delete(const IConfig &config, const HTTPRequest *request,
                              const std::string &port, const std::string &ip);
  static http::StatusCode CGIHandler(const IConfig &config,
                                     RequestContext req_ctx, const std::string &path);

 private:
  RequestHandler();
  RequestHandler(const RequestHandler &other);
  RequestHandler &operator=(const RequestHandler &other);
  ~RequestHandler();

  static const int kMaxUriLength = 2048;

  static HTTPResponse *GenerateAutoIndexPage(const IConfig &config,
                                             const HTTPRequest *request,
                                             const std::string &abs_path);
  static std::map<std::string, std::string> GetEnv(const IConfig &config, const RequestContext &req_ctx);
  static char **DupEnv(const std::map<std::string, std::string> &env_map);
  static void DeleteEnv(char **env);
};

#endif  // WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_
