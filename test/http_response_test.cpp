#include "http_response.hpp"

#include <gtest/gtest.h>

#include <fakeit.hpp>

using namespace fakeit;

TEST(HTTPResponseBuilderTest, BuildResponse) {
  HTTPResponse *res = HTTPResponse::Builder()
                          .SetStatusCode(http::kOk)
                          .AddHeader("Content-Type", "text/plain")
                          .SetBody("Hello, World!")
                          .Build();
  std::string expected =
      "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: "
      "13\r\n\r\nHello, World!";
  ASSERT_EQ(res->ToString(), expected);
  delete res;
}

TEST(HTTPResponseBuilderTest, BuildResponseWithoutStatus) {
  HTTPResponse *res = HTTPResponse::Builder()
                          .AddHeader("Content-Type", "text/plain")
                          .SetBody("Hello, World!")
                          .Build();
  ASSERT_EQ(res->GetStatusCode(), http::kInternalServerError);
}

TEST(GenerateErrorResponse, DefaultNotFound) {
  Mock<IConfig> mock;
  std::map<std::string, std::string> error_page = {};
  When(Method(mock, GetErrorPage))
      .AlwaysReturn(error_page);

  HTTPResponse *res = GenerateErrorResponse(http::kNotFound, mock.get());
  const std::string expected =
      "<html>\r\n<head><title>404 Not "
      "Found</title></head>\r\n<body>\r\n<center><h1>404 Not "
      "Found</h1></center>\r\n<hr><center>webserver</center>\r\n</body>\r\n";
  ASSERT_EQ(res->GetBody(), expected);
  delete res;
}

TEST(GenerateErrorResponse, ProvidedNotFound) {
  Mock<IConfig> mock;
  std::map<std::string, std::string> error_pages = {
      {"404", "test/error_page/404.html"}};
  When(Method(mock, GetErrorPage)).AlwaysReturn(error_pages);

  HTTPResponse *res = GenerateErrorResponse(http::kNotFound, mock.get());
  const std::string expected = "<h1>404 Not Found</h1>";
  ASSERT_EQ(res->GetBody(), expected);
  delete res;
}

TEST(GenerateErrorResponse, ProvidedInvalidErrorPage) {
  Mock<IConfig> mock;
  std::map<std::string, std::string> error_pages = {
      {"404", "test/error_page/invalid.html"}};
  When(Method(mock, GetErrorPage)).AlwaysReturn(error_pages);

  HTTPResponse *res = GenerateErrorResponse(http::kNotFound, mock.get());
  const std::string expected =
      "<html>\r\n<head><title>404 Not "
      "Found</title></head>\r\n<body>\r\n<center><h1>404 Not "
      "Found</h1></center>\r\n<hr><center>webserver</center>\r\n</body>\r\n";
  ASSERT_EQ(res->GetBody(), expected);
  delete res;
}

TEST(GenerateErrorResponse, ProvidedValidButDirectoryErrorPage) {
  Mock<IConfig> mock;
  std::map<std::string, std::string> error_pages = {
      {"404", "test/error_page/"}};
  When(Method(mock, GetErrorPage)).AlwaysReturn(error_pages);

  HTTPResponse *res = GenerateErrorResponse(http::kNotFound, mock.get());
  const std::string expected =
      "<html>\r\n<head><title>404 Not "
      "Found</title></head>\r\n<body>\r\n<center><h1>404 Not "
      "Found</h1></center>\r\n<hr><center>webserver</center>\r\n</body>\r\n";
  ASSERT_EQ(res->GetBody(), expected);
  delete res;
}
