#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>

#include "http_request.hpp"
#include "http_request_parser.hpp"

// kEndParseのテスト
TEST(HTTPRequestParser, kEndParse) {
  std::string request = "";
  HTTPRequestParser parser;
  const Result<HTTPRequest *, int> req = parser.Parser(request);
  EXPECT_EQ(req.UnwrapErr(), HTTPRequestParser::kEndParse);
}

// GETリクエストのパース(ちょっと特殊なタイプ）
TEST(HTTPRequestParser, ParseRequestGET) {
  // valueの後ろに無駄に空白とか
  std::string request = "GET / HTTP/1.1\r\nHost: localhost:8080    \r\n\r\n";
  HTTPRequestParser parser;
  const Result<HTTPRequest *, int> req = parser.Parser(request);
  EXPECT_EQ(req.Unwrap()->GetMethod(), "GET");
  EXPECT_EQ(req.Unwrap()->GetUri(), "/");
  EXPECT_EQ(req.Unwrap()->GetProtocol(), "HTTP");
  EXPECT_EQ(req.Unwrap()->GetVersion(), "1.1");
  EXPECT_EQ(req.Unwrap()->GetHostHeader(), "LOCALHOST:8080");
  EXPECT_EQ(req.Unwrap()->GetBody(), "");
  delete req.Unwrap();
  // queryがある時
  request = "GET /aaaaa?bbbbb HTTP/1.1\r\nHost: localhost:8080    \r\n\r\n";
  const Result<HTTPRequest *, int> req1 = parser.Parser(request);
  EXPECT_EQ(req1.Unwrap()->GetMethod(), "GET");
  EXPECT_EQ(req1.Unwrap()->GetUri(), "/aaaaa");
  EXPECT_EQ(req1.Unwrap()->GetQuery(), "bbbbb");
  EXPECT_EQ(req1.Unwrap()->GetProtocol(), "HTTP");
  EXPECT_EQ(req1.Unwrap()->GetVersion(), "1.1");
  EXPECT_EQ(req1.Unwrap()->GetHostHeader(), "LOCALHOST:8080");
  EXPECT_EQ(req1.Unwrap()->GetBody(), "");
  delete req.Unwrap();
  // Hostの中身がない
  request = "GET / HTTP/1.1\r\nHost:\r\n\r\n";
  Result<HTTPRequest *, int> req9 = parser.Parser(request);
  EXPECT_EQ(req9.Unwrap()->GetMethod(), "GET");
  delete req9.Unwrap();
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
  delete req1.Unwrap();
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
  // Hostヘッダが2つある
  request =
      "GET / HTTP/1.1\r\nHost: localhost:8080\r\nHost: localhost:8080\r\n\r\n";
  Result<HTTPRequest *, int> req5 = parser.Parser(request);
  EXPECT_EQ(req5.UnwrapErr(), HTTPRequestParser::kBadRequest);
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
  delete req2.Unwrap();
}

// POSTで次のリクエストもきてた時のパース
TEST(HTTPRequestParser, ParseRequestPOST_Contentlength_twice) {
  HTTPRequestParser parser;
  std::string request =
      "POST / HTTP/1.1\r\nHost: localhost:8080\r\nContent-Length: 5\r\n\r\n";
  Result<HTTPRequest *, int> req = parser.Parser(request);
  EXPECT_EQ(req.UnwrapErr(), HTTPRequestParser::kNotEnough);
  request = "he";
  Result<HTTPRequest *, int> req1 = parser.Parser(request);
  EXPECT_EQ(req1.UnwrapErr(), HTTPRequestParser::kNotEnough);
  request =
      "lloPOST / HTTP/1.1\r\nHost: localhost:8080\r\nContent-Length: "
      "5\r\n\r\nhello";
  Result<HTTPRequest *, int> req2 = parser.Parser(request);
  EXPECT_EQ(req2.Unwrap()->GetMethod(), "POST");
  EXPECT_EQ(req2.Unwrap()->GetUri(), "/");
  EXPECT_EQ(req2.Unwrap()->GetProtocol(), "HTTP");
  EXPECT_EQ(req2.Unwrap()->GetVersion(), "1.1");
  EXPECT_EQ(req2.Unwrap()->GetHostHeader(), "LOCALHOST:8080");
  EXPECT_EQ(req2.Unwrap()->GetBody(), "hello");
  delete req2.Unwrap();
  request = "";
  Result<HTTPRequest *, int> req3 = parser.Parser(request);
  EXPECT_EQ(req3.Unwrap()->GetMethod(), "POST");
  EXPECT_EQ(req3.Unwrap()->GetUri(), "/");
  EXPECT_EQ(req3.Unwrap()->GetProtocol(), "HTTP");
  EXPECT_EQ(req3.Unwrap()->GetVersion(), "1.1");
  EXPECT_EQ(req3.Unwrap()->GetHostHeader(), "LOCALHOST:8080");
  EXPECT_EQ(req3.Unwrap()->GetBody(), "hello");
  delete req3.Unwrap();
  request = "";
  Result<HTTPRequest *, int> req4 = parser.Parser(request);
  EXPECT_EQ(req4.UnwrapErr(), HTTPRequestParser::kEndParse);
  request = "";
  Result<HTTPRequest *, int> req6 = parser.Parser(request);
  EXPECT_EQ(req6.UnwrapErr(), HTTPRequestParser::kEndParse);
}

// Transfer-Encodingのパース
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
  delete req2.Unwrap();
}

// Transfer-Encodingのパース(16進数の場合)
TEST(HTTPRequestParser, ParseRequestPOST_Transfer_chunked_Hex) {
  HTTPRequestParser parser;
  std::string request =
      "POST / HTTP/1.1\r\nHost: localhost:8080\r\nTransfer-Encoding: "
      "chunked\r\n\r\n";
  Result<HTTPRequest *, int> req = parser.Parser(request);
  EXPECT_EQ(req.UnwrapErr(), HTTPRequestParser::kNotEnough);
  request = "a\r\n";
  Result<HTTPRequest *, int> req1 = parser.Parser(request);
  EXPECT_EQ(req1.UnwrapErr(), HTTPRequestParser::kNotEnough);
  request = "hellohello\r\n";
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
  EXPECT_EQ(req2.Unwrap()->GetBody(), "hellohello");
  delete req2.Unwrap();
}
//// Transfer-Encodingのパース 2度の処理がくる場合
TEST(HTTPRequestParser, ParseRequestPOST_Transfer_chunked2) {
  HTTPRequestParser parser;
  std::string request =
      "POST / HTTP/1.1\r\nHost: localhost:8080\r\nTransfer-Encoding: "
      "chunked\r\n\r\n5\r\nhello\r\n0\r\n\r\n";
  Result<HTTPRequest *, int> req2 = parser.Parser(request);
  EXPECT_EQ(req2.Unwrap()->GetMethod(), "POST");
  EXPECT_EQ(req2.Unwrap()->GetUri(), "/");
  EXPECT_EQ(req2.Unwrap()->GetProtocol(), "HTTP");
  EXPECT_EQ(req2.Unwrap()->GetVersion(), "1.1");
  EXPECT_EQ(req2.Unwrap()->GetHostHeader(), "LOCALHOST:8080");
  EXPECT_EQ(req2.Unwrap()->GetBody(), "hello");
  delete req2.Unwrap();
  request =
      "POST / HTTP/1.1\r\nHost: localhost:8080\r\nTransfer-Encoding: "
      "chunked\r\n\r\n5\r\nhello\r\n0\r\n\r\n";
  Result<HTTPRequest *, int> req3 = parser.Parser(request);
  EXPECT_EQ(req3.Unwrap()->GetMethod(), "POST");
  EXPECT_EQ(req3.Unwrap()->GetUri(), "/");
  EXPECT_EQ(req3.Unwrap()->GetProtocol(), "HTTP");
  EXPECT_EQ(req3.Unwrap()->GetVersion(), "1.1");
  EXPECT_EQ(req3.Unwrap()->GetHostHeader(), "LOCALHOST:8080");
  EXPECT_EQ(req3.Unwrap()->GetBody(), "hello");
  delete req3.Unwrap();
}

// Transfer-Encodingのエラーケース数字来ないで終わる
TEST(HTTPRequestParser, ParseRequestPOST_Transfer_chunked_Error1) {
  HTTPRequestParser parser;
  std::string request =
      "POST / HTTP/1.1\r\nHost: localhost:8080\r\nTransfer-Encoding: "
      "chunked\r\n\r\n";
  Result<HTTPRequest *, int> req = parser.Parser(request);
  EXPECT_EQ(req.UnwrapErr(), HTTPRequestParser::kNotEnough);
  request = "\r\n";
  Result<HTTPRequest *, int> req1 = parser.Parser(request);
  EXPECT_EQ(req1.UnwrapErr(), HTTPRequestParser::kBadRequest);
}

// Transfer-Encodingのエラーケース数字が来ても0\r\nがない
TEST(HTTPRequestParser, ParseRequestPOST_Transfer_chunked_Error2) {
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
  request = "\r\n\r\n";
  Result<HTTPRequest *, int> req5 = parser.Parser(request);
  EXPECT_EQ(req5.UnwrapErr(), HTTPRequestParser::kBadRequest);
}

// Transfer-Encodingのエラーケース: チャンクの文字数が多い
TEST(HTTPRequestParser, ParseRequestPOST_Transfer_chunked_Error3) {
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
  request = "0\r\naaaaaa\r\n";
  Result<HTTPRequest *, int> req5 = parser.Parser(request);
  EXPECT_EQ(req5.UnwrapErr(), HTTPRequestParser::kBadRequest);
}

// Transfer-Encodingのエラーケース: チャンクの文字数が少ない
TEST(HTTPRequestParser, ParseRequestPOST_Transfer_chunked_Error4) {
  HTTPRequestParser parser;
  std::string request =
      "POST / HTTP/1.1\r\nHost: localhost:8080\r\nTransfer-Encoding: "
      "chunked\r\n\r\n";
  Result<HTTPRequest *, int> req = parser.Parser(request);
  EXPECT_EQ(req.UnwrapErr(), HTTPRequestParser::kNotEnough);
  request = "5\r\n";
  Result<HTTPRequest *, int> req1 = parser.Parser(request);
  EXPECT_EQ(req1.UnwrapErr(), HTTPRequestParser::kNotEnough);
  request = "hel\r\n";
  Result<HTTPRequest *, int> req3 = parser.Parser(request);
  EXPECT_EQ(req3.UnwrapErr(), HTTPRequestParser::kBadRequest);
}

// Transfer-Encodingとcontent-lengthが両方ある場合
TEST(HTTPRequestParser, ParseRequestPOST_Transfer_chunked_ContentLength) {
  HTTPRequestParser parser;
  std::string request =
      "POST / HTTP/1.1\r\nHost: localhost:8080\r\nTransfer-Encoding: "
      "chunked\r\nContent-Length: 5\r\n\r\n";
  Result<HTTPRequest *, int> req = parser.Parser(request);
  EXPECT_EQ(req.UnwrapErr(), HTTPRequestParser::kBadRequest);
}
