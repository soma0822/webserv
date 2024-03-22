#include "http_response.hpp"

#include <gtest/gtest.h>

#include <fakeit.hpp>

using namespace fakeit;

TEST(HTTPResponseBuilderTest, BuildResponse) {
  Option<HTTPResponse *> opt = HTTPResponse::Builder()
                               .SetStatusCode(http::kOk)
                               .AddHeader("Content-Type", "text/plain")
                               .SetBody("Hello, World!")
                               .Build();
  std::string expected =
      "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";
  ASSERT_EQ(opt.Unwrap()->ToString(), expected);
  delete opt.Unwrap();
}

TEST(HTTPResponseBuilderTest, BuildResponseWithoutStatus) {
  Option<HTTPResponse *> opt = HTTPResponse::Builder()
                               .AddHeader("Content-Type", "text/plain")
                               .SetBody("Hello, World!")
                               .Build();
  ASSERT_EQ(opt.Unwrap(), (HTTPResponse *)NULL);
}

TEST(GenerateErrorResponse, DefaultNotFound) {
  Mock<IConfig> mock;
  When(Method(mock, GetErrorPage))
      .AlwaysReturn(std::map<std::string, std::string>{});

  Option<HTTPResponse *> opt = GenerateErrorResponse(http::kNotFound, mock.get());
  const std::string expected =
      "<html>\r\n<head><title>404 Not "
      "Found</title></head>\r\n<body>\r\n<center><h1>404 Not "
      "Found</h1></center>\r\n<hr><center>webserver</center>\r\n</body>\r\n";
  ASSERT_EQ(opt.Unwrap()->GetBody(), expected);
  delete opt.Unwrap();
}

TEST(GenerateErrorResponse, ProvidedNotFound) {
  Mock<IConfig> mock;
  std::map<std::string, std::string> error_pages = {
      {"404", "test/error_page/404.html"}};
  When(Method(mock, GetErrorPage)).AlwaysReturn(error_pages);

  Option<HTTPResponse *> opt = GenerateErrorResponse(http::kNotFound, mock.get());
  const std::string expected = "<h1>404 Not Found</h1>";
  ASSERT_EQ(opt.Unwrap()->GetBody(), expected);
  delete opt.Unwrap();
}

TEST(GenerateErrorResponse, ProvidedInvalidErrorPage) {
  Mock<IConfig> mock;
  std::map<std::string, std::string> error_pages = {
      {"404", "test/error_page/invalid.html"}};
  When(Method(mock, GetErrorPage)).AlwaysReturn(error_pages);

  Option<HTTPResponse *> opt = GenerateErrorResponse(http::kNotFound, mock.get());
  const std::string expected =
      "<html>\r\n<head><title>404 Not "
      "Found</title></head>\r\n<body>\r\n<center><h1>404 Not "
      "Found</h1></center>\r\n<hr><center>webserver</center>\r\n</body>\r\n";
  ASSERT_EQ(opt.Unwrap()->GetBody(), expected);
  delete opt.Unwrap();
}

TEST(GenerateErrorResponse, ProvidedValidButDirectoryErrorPage) {
  Mock<IConfig> mock;
  std::map<std::string, std::string> error_pages = {
      {"404", "test/error_page/"}};
  When(Method(mock, GetErrorPage)).AlwaysReturn(error_pages);

  Option<HTTPResponse *> opt = GenerateErrorResponse(http::kNotFound, mock.get());
  const std::string expected =
      "<html>\r\n<head><title>404 Not "
      "Found</title></head>\r\n<body>\r\n<center><h1>404 Not "
      "Found</h1></center>\r\n<hr><center>webserver</center>\r\n</body>\r\n";
  ASSERT_EQ(opt.Unwrap()->GetBody(), expected);
  delete opt.Unwrap();
}
