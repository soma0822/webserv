#include "cgi_parser.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>

#include "http_request.hpp"

// CGIParserの基本的なテスト
TEST(CGIParser, CGIParserNomalRequest) {
  std::string request = "Host: localhost:8080    \r\n\r\naaaaa\r\n";
  CGIParser parser;
  const Result<HTTPRequest *, int> req = parser.Parser(request);
  EXPECT_EQ(req.Unwrap()->GetHeaders().find("HOST")->second, "LOCALHOST:8080");
  EXPECT_EQ(req.Unwrap()->GetBody(), "aaaaa\r\n");
  delete req.Unwrap();
}
