#include "cgi_handler.hpp"

Option<HTTPResponse *> CGIHandler::Handle(const IConfig &config,
                                          HTTPRequest *cgi_req,
                                          RequestContext req_ctx) {
  if (cgi_req->GetHeaders().count("LOCATION") == 1 &&
      cgi_req->GetHeaders().at("LOCATION").at(0) == '/') {
    if (req_ctx.count >= 10)
      return Some(GenerateErrorResponse(http::kInternalServerError, config));
    ++req_ctx.count;
    std::string location = cgi_req->GetHeaders().at("LOCATION");
    req_ctx.request->SetUri(location);
    return RequestHandler::Handle(config, req_ctx);
  }
  HTTPResponse *res = new HTTPResponse;
  if (cgi_req->GetHeaders().count("STATUS") == 0) {
    res->SetStatusCode(http::kOk);
  }
  for (std::map<std::string, std::string>::const_iterator it =
           cgi_req->GetHeaders().begin();
       it != cgi_req->GetHeaders().end(); ++it) {
    if (it->first == "STATUS") {
      std::stringstream ss(it->second);
      std::string status_code;
      ss >> status_code;
      Result<int, std::string> result = string_utils::StrToI(status_code);
      if (result.IsErr()) {
        delete res;
        return Some(GenerateErrorResponse(http::kInternalServerError, config));
      }
      res->SetStatusCode(static_cast<http::StatusCode>(result.Unwrap()));
    } else {
      res->AddHeader(it->first, it->second);
    }
  }
  res->SetHTTPVersion("HTTP/1.1");
  res->SetBody(cgi_req->GetBody());
  return Some(res);
}
