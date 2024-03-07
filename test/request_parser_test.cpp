#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>

#include "http_request.hpp"
#include "http_request_parser.hpp"

// GETリクエストのパース
TEST(HTTPRequestParser, ParseRequestGET) {
  std::string request = "GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n";
  HTTPRequestParser parser;
  const Result<HTTPRequest *, int> req = parser.Parser(request);
  EXPECT_EQ(req.Unwrap()->GetMethod(), "GET");
  EXPECT_EQ(req.Unwrap()->GetUri(), "/");
  EXPECT_EQ(req.Unwrap()->GetProtocol(), "HTTP");
  EXPECT_EQ(req.Unwrap()->GetVersion(), "1.1");
  EXPECT_EQ(req.Unwrap()->GetHostHeader(), "LOCALHOST:8080");
  EXPECT_EQ(req.Unwrap()->GetBody(), "");
}

// GETリクエストのパース(ヘッダがバラバラに送られてくる場合）
TEST(HTTPRequestParser, ParseRequestGET_Header_kNotEnough) {
  HTTPRequestParser parser;
  std::string request = "GET / HTTP/1.1\r\n";
  Result<HTTPRequest *, int> req = parser.Parser(request);
  EXPECT_EQ(req.UnwrapErr(), HTTPRequestParser::kNotEnough);
  request = "Host: localhost:8080\r\n";
  Result<HTTPRequest *, int> req2 = parser.Parser(request);
  EXPECT_EQ(req2.UnwrapErr(), HTTPRequestParser::kNotEnough);
  request = "\r\n";
  Result<HTTPRequest *, int> req1 = parser.Parser(request);
  EXPECT_EQ(req1.Unwrap()->GetMethod(), "GET");
  EXPECT_EQ(req1.Unwrap()->GetUri(), "/");
  EXPECT_EQ(req1.Unwrap()->GetProtocol(), "HTTP");
  EXPECT_EQ(req1.Unwrap()->GetVersion(), "1.1");
  EXPECT_EQ(req1.Unwrap()->GetHostHeader(), "LOCALHOST:8080");
  EXPECT_EQ(req1.Unwrap()->GetBody(), "");
}

// requestlineエラーケース
TEST(HTTPRequestParser, ParseRequestGET_Requestline_BadRequest) {
  HTTPRequestParser parser;

  // methodがない
  std::string request = " / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n";
  Result<HTTPRequest *, int> req = parser.Parser(request);
  EXPECT_EQ(req.UnwrapErr(), HTTPRequestParser::kBadRequest);
  // pathがない
  request = "GET HTTP/1.1\r\nHost: localhost:8080\r\n\r\n";
  Result<HTTPRequest *, int> req3 = parser.Parser(request);
  EXPECT_EQ(req3.UnwrapErr(), HTTPRequestParser::kBadRequest);
  // protocolがない
  request = "GET / /1.1\r\nHost: localhost:8080\r\n\r\n";
  Result<HTTPRequest *, int> req7 = parser.Parser(request);
  EXPECT_EQ(req7.UnwrapErr(), HTTPRequestParser::kBadRequest);
  // protocolとversionの間の/がない
  request = "GET / HTTP1.1\r\nHost: localhost:8080\r\n\r\n";
  Result<HTTPRequest *, int> req4 = parser.Parser(request);
  EXPECT_EQ(req4.UnwrapErr(), HTTPRequestParser::kBadRequest);
  // versionがない
  request = "GET / HTTP/\r\nHost: localhost:8080\r\n\r\n";
  Result<HTTPRequest *, int> req5 = parser.Parser(request);
  EXPECT_EQ(req5.UnwrapErr(), HTTPRequestParser::kBadRequest);
  //\r\nがない
  request = "GET / HTTP/1.1Host: localhost:8080\r\n\r\n";
  Result<HTTPRequest *, int> req6 = parser.Parser(request);
  EXPECT_EQ(req6.UnwrapErr(), HTTPRequestParser::kBadRequest);
}

// headerエラーケース
TEST(HTTPRequestParser, ParseRequestGET_Header_BadRequest) {
  HTTPRequestParser parser;
  std::string request;
  // Hostヘッダがない
  request = "GET / HTTP/1.1\r\n\r\n";
  Result<HTTPRequest *, int> req7 = parser.Parser(request);
  EXPECT_EQ(req7.UnwrapErr(), HTTPRequestParser::kBadRequest);
  // Hostヘッダがない
  request = "GET / HTTP/1.1\r\n: localhost:8080\r\n\r\n";
  Result<HTTPRequest *, int> req8 = parser.Parser(request);
  EXPECT_EQ(req8.UnwrapErr(), HTTPRequestParser::kBadRequest);
  // Hostの中身がない
  request = "GET / HTTP/1.1\r\nHost:\r\n\r\n";
  Result<HTTPRequest *, int> req9 = parser.Parser(request);
  EXPECT_EQ(req9.UnwrapErr(), HTTPRequestParser::kBadRequest);
  // Hostがない
  request =
      "GET / HTTP/1.1\r\nIf-Modified-Since: Thu, "
      "14 Jun 2018 10:00:00 GMT\r\n\r\n";
  Result<HTTPRequest *, int> req6 = parser.Parser(request);
  EXPECT_EQ(req6.UnwrapErr(), HTTPRequestParser::kBadRequest);
}

// POSTリクエストのパース
TEST(HTTPRequestParser, ParseRequestPOST) {
  HTTPRequestParser parser;
  std::string request =
      "POST / HTTP/1.1\r\nHost: localhost:8080\r\nContent-Length: 5\r\n\r\n";
  Result<HTTPRequest *, int> req = parser.Parser(request);
  EXPECT_EQ(req.UnwrapErr(), HTTPRequestParser::kNotEnough);
  request = "he";
  Result<HTTPRequest *, int> req1 = parser.Parser(request);
  EXPECT_EQ(req1.UnwrapErr(), HTTPRequestParser::kNotEnough);
  request = "llo\r\n";
  Result<HTTPRequest *, int> req2 = parser.Parser(request);
  EXPECT_EQ(req2.Unwrap()->GetMethod(), "POST");
  EXPECT_EQ(req2.Unwrap()->GetUri(), "/");
  EXPECT_EQ(req2.Unwrap()->GetProtocol(), "HTTP");
  EXPECT_EQ(req2.Unwrap()->GetVersion(), "1.1");
  EXPECT_EQ(req2.Unwrap()->GetHostHeader(), "LOCALHOST:8080");
  EXPECT_EQ(req2.Unwrap()->GetBody(), "hello");
}

// POSTリクエストのパース
TEST(HTTPRequestParser, ParseRequestPOST_Transfer_chunked) {
  HTTPRequestParser parser;
  std::string request =
      "POST / HTTP/1.1\r\nHost: localhost:8080\r\nTransfer-Encoding: "
      "chunked\r\n\r\n";
  Result<HTTPRequest *, int> req = parser.Parser(request);
  EXPECT_EQ(req.UnwrapErr(), HTTPRequestParser::kNotEnough);
  request = "5\r\n";
  Result<HTTPRequest *, int> req1 = parser.Parser(request);
  EXPECT_EQ(req1.UnwrapErr(), HTTPRequestParser::kNotEnough);
  request = "hello\r\n";
  Result<HTTPRequest *, int> req3 = parser.Parser(request);
  EXPECT_EQ(req3.UnwrapErr(), HTTPRequestParser::kNotEnough);
  request = "0\r\n";
  Result<HTTPRequest *, int> req4 = parser.Parser(request);
  EXPECT_EQ(req4.UnwrapErr(), HTTPRequestParser::kNotEnough);
  request = "\r\n";
  Result<HTTPRequest *, int> req2 = parser.Parser(request);
  EXPECT_EQ(req2.Unwrap()->GetMethod(), "POST");
  EXPECT_EQ(req2.Unwrap()->GetUri(), "/");
  EXPECT_EQ(req2.Unwrap()->GetProtocol(), "HTTP");
  EXPECT_EQ(req2.Unwrap()->GetVersion(), "1.1");
  EXPECT_EQ(req2.Unwrap()->GetHostHeader(), "LOCALHOST:8080");
  EXPECT_EQ(req2.Unwrap()->GetBody(), "hello");
}
