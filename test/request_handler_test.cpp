#include "request_handler.hpp"

#include <gtest/gtest.h>

#include <fstream>

TEST(RequestHandlerTest, GetMethodNormal) {
  const std::string test_file_path = "/tmp/index.html";
  std::ofstream ofs(test_file_path.c_str());
  ofs << "Hello, world!" << std::endl;
  ofs.close();

  HTTPRequest request;
  const HTTPResponse *response =
      RequestHandler::Get(&request, "/tmp/index.html");

  ASSERT_EQ(response->GetStatusCode(), 200);
  ASSERT_EQ(response->GetBody(), "Hello, world!\n");

  unlink(test_file_path.c_str());

  delete response;
}
