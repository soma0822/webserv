#include "request_handler.hpp"

#include <gtest/gtest.h>

#include <fakeit.hpp>
#include <fstream>

#include "logger.hpp"

using namespace fakeit;

TEST(RequestHandlerTest, GetMethodNormal) {
  const std::string root = "/tmp";
  const std::string uri = "/index.html";
  const std::string test_file_path = root + uri;
  std::ofstream ofs(test_file_path.c_str());
  ofs << "Hello, world!" << std::endl;
  ofs.close();

  LocationContext ctx;
  ctx.SetCanAutoIndex(true);
  Mock<IServerContext> mock;
  When(Method(mock, SearchLocation))
      .AlwaysReturn(Result<LocationContext, std::string>(Ok(ctx)));
  When(Method(mock, GetRoot)).AlwaysReturn(root);
  When(Method(mock, GetIndex)).AlwaysReturn(uri.substr(1));

  HTTPRequest request;
  request.SetUri(uri);
  const HTTPResponse *response = RequestHandler::Get(mock.get(), &request);

  ASSERT_EQ(response->GetStatusCode(), 200);
  ASSERT_EQ(response->GetBody(), "Hello, world!\n");

  unlink(test_file_path.c_str());

  delete response;
}
