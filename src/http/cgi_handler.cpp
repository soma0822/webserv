#include "cgi_handler.hpp"

Option<HTTPResponse *> CGIHandler::Handle(const IConfig &config,
                                          HTTPRequest *cgi_req,
                                          RequestContext req_ctx) {
  if (cgi_req->GetHeaders().count("LOCATION") == 1 &&
      cgi_req->GetHeaders().at("LOCATION").at(0) == '/') {
    if (req_ctx.count >= 10)
      return (Some(HTTPResponse::Builder()
                       .SetStatusCode(http::kInternalServerError)
                       .Build()));
    req_ctx.count++;
    std::string location = cgi_req->GetHeaders().at("LOCATION");
    req_ctx.request->SetUri(location);
    return RequestHandler::Handle(config, req_ctx);
  }
  HTTPResponse::Builder res_builder = HTTPResponse::Builder();
  if (cgi_req->GetHeaders().count("STATUS") == 0) {
    res_builder.SetStatusCode(http::kOk);
  }
  for (std::map<std::string, std::string>::const_iterator it =
           cgi_req->GetHeaders().begin();
       it != cgi_req->GetHeaders().end(); it++) {
    if (it->first == "STATUS") {
      res_builder.SetStatusCode(
          static_cast<http::StatusCode>(std::stoi(it->second)));
    } else {
      res_builder.AddHeader(it->first, it->second);
    }
  }
  return (Some(res_builder.SetBody(cgi_req->GetBody()).Build()));
}
