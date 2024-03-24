#include "cgi_handler.hpp"

#include <gtest/gtest.h>

#include "cgi_parser.hpp"
#include "file_utils.hpp"
#include "result.hpp"

TEST(CGIHandler, CGINomal) {
  Config config;
  std::string str =
      "Status: 200\r\nContent-Type: text/html\r\ncharset: UTF-8\r\n\r\n<html>";
  CGIParser parser;
  HTTPRequest* req = parser.Parser(str).Unwrap();
  RequestContext req_ctx;
  HTTPResponse* res = CGIHandler::Handle(config, req, req_ctx).Unwrap();
  ASSERT_EQ("<html>", res->GetBody());
  ASSERT_EQ(http::kOk, res->GetStatusCode());
  ASSERT_EQ("text/html", res->GetHeaders().at("CONTENT-TYPE"));
  ASSERT_EQ("UTF-8", res->GetHeaders().at("CHARSET"));
  delete res;
  delete req;
}

TEST(CGIHandler, CGIStatusCut) {
  Config config;
  std::string str = "Content-Type: text/html\r\ncharset: UTF-8\r\n\r\n<html>";
  CGIParser parser;
  HTTPRequest* req = parser.Parser(str).Unwrap();
  RequestContext req_ctx;
  HTTPResponse* res = CGIHandler::Handle(config, req, req_ctx).Unwrap();
  ASSERT_EQ("<html>", res->GetBody());
  ASSERT_EQ(http::kOk, res->GetStatusCode());
  ASSERT_EQ("text/html", res->GetHeaders().at("CONTENT-TYPE"));
  ASSERT_EQ("UTF-8", res->GetHeaders().at("CHARSET"));
  delete res;
  delete req;
}

TEST(CGIHandler, CGIStatusInvalid) {
  Config config;
  std::string str =
      "Status: aaa\r\nContent-Type: text/html\r\ncharset: UTF-8\r\n\r\n<html>";
  CGIParser parser;
  HTTPRequest* req = parser.Parser(str).Unwrap();
  RequestContext req_ctx;
  HTTPResponse* res = CGIHandler::Handle(config, req, req_ctx).Unwrap();
  ASSERT_EQ(http::kInternalServerError, res->GetStatusCode());
  delete res;
  delete req;
}
