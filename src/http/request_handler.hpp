#ifndef WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_
#define WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_

#include "config.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "io_task_manager.hpp"
#include "option.hpp"
#include "read_from_cgi.hpp"
#include "time.hpp"
#include "write_to_cgi.hpp"
#include "file_utils.hpp"

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
                                 const std::string &script_name,
                                 const std::string &path_translated);
  static std::string GetCGIScriptPath(const IConfig &config,
                                      RequestContext req_ctx);
  static std::string GetPathInfoPath(const IConfig &config,
                                     RequestContext req_ctx);
  static char const **MakeArgv(const std::string &script_name,
                               std::string &first_line);

 private:
  RequestHandler();
  RequestHandler(const RequestHandler &other);
  RequestHandler &operator=(const RequestHandler &other);
  ~RequestHandler();

  static const int kMaxUriLength = 2048;

  static HTTPResponse *GenerateAutoIndexPage(const IConfig &config,
                                             const HTTPRequest *request,
                                             const std::string &abs_path);
  static std::map<std::string, std::string> GetEnv(
      const IConfig &config, const RequestContext &req_ctx,
      const std::string &script_name, const std::string &path_translated);
  static char **DupEnv(const std::map<std::string, std::string> &env_map);
  static void DeleteEnv(char **env);
  static std::pair<std::string, std::string> ResolveRequestTargetPath(const IConfig &config,
                                              const RequestContext req_ctx);
  static std::string ResolveRootPath(const IConfig &config,
                                     const RequestContext req_ctx);
  static std::string ResolveScriptPart(const std::string &target);
  static std::string RemoveLocPath(const IConfig &config, const RequestContext req_ctx);
  static bool IsCGIRequest(const IConfig &config, RequestContext req_ctx);
  static bool IsAllowedMethod(const IConfig &config, RequestContext req_ctx);
  static const std::string server_dir;
};

#endif  // WEBSERV_SRC_HTTP_REQUEST_HANDLER_HPP_
