#include "http_response.hpp"

#include <gtest/gtest.h>

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
