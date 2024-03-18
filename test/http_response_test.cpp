#include "http_response.hpp"

#include <gtest/gtest.h>

#include <fakeit.hpp>

using namespace fakeit;

TEST(HTTPResponseBuilderTest, BuildResponse) {
  HTTPResponse *response = HTTPResponse::Builder()
                               .SetStatusCode(http::kOk)
                               .AddHeader("Content-Type", "text/plain")
                               .SetBody("Hello, World!")
                               .Build();
  std::string expected =
      "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";
  ASSERT_EQ(response->ToString(), expected);
  delete response;
}

TEST(HTTPResponseBuilderTest, BuildResponseWithoutStatus) {
  HTTPResponse *response = HTTPResponse::Builder()
                               .AddHeader("Content-Type", "text/plain")
                               .SetBody("Hello, World!")
                               .Build();
  ASSERT_EQ(response, (HTTPResponse *)NULL);
}

TEST(GenerateErrorResponse, DefaultNotFound) {
  Mock<IConfig> mock;
  When(Method(mock, GetErrorPage))
      .AlwaysReturn(std::map<std::string, std::string>{});

  HTTPResponse *response = GenerateErrorResponse(http::kNotFound, mock.get());
  const std::string expected =
      "<html>\r\n<head><title>404 Not "
      "Found</title></head>\r\n<body>\r\n<center><h1>404 Not "
      "Found</h1></center>\r\n<hr><center>webserver</center>\r\n</body>\r\n";
  ASSERT_EQ(response->GetBody(), expected);
  delete response;
}

TEST(GenerateErrorResponse, ProvidedNotFound) {
  Mock<IConfig> mock;
  std::map<std::string, std::string> error_pages = {
      {"404", "test/error_page/404.html"}};
  When(Method(mock, GetErrorPage)).AlwaysReturn(error_pages);

  HTTPResponse *response = GenerateErrorResponse(http::kNotFound, mock.get());
  const std::string expected = "<h1>404 Not Found</h1>";
  ASSERT_EQ(response->GetBody(), expected);
  delete response;
}

TEST(GenerateErrorResponse, ProvidedInvalidErrorPage) {
  Mock<IConfig> mock;
  std::map<std::string, std::string> error_pages = {
      {"404", "test/error_page/invalid.html"}};
  When(Method(mock, GetErrorPage)).AlwaysReturn(error_pages);

  HTTPResponse *response = GenerateErrorResponse(http::kNotFound, mock.get());
  const std::string expected =
      "<html>\r\n<head><title>404 Not "
      "Found</title></head>\r\n<body>\r\n<center><h1>404 Not "
      "Found</h1></center>\r\n<hr><center>webserver</center>\r\n</body>\r\n";
  ASSERT_EQ(response->GetBody(), expected);
  delete response;
}
