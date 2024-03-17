#include "request_handler.hpp"

#include <gtest/gtest.h>

#include <fakeit.hpp>
#include <fstream>

#include "http_status_code.hpp"

using namespace fakeit;

class RequestHandlerTest : public testing::Test {
 protected:
  HTTPResponse *response = nullptr;
  const std::string test_file_path = "/tmp/index.html";
  void TearDown() override {
    unlink(test_file_path.c_str());
    delete response;
  }
};

TEST_F(RequestHandlerTest, GetMethodNormal) {
  const std::string root = "/tmp";
  const std::string uri = "/index.html";
  const std::string test_file_path = root + uri;
  std::ofstream ofs(test_file_path.c_str());
  ofs << "Hello, world!" << std::endl;
  ofs.close();

  LocationContext ctx;
  ctx.SetCanAutoIndex(true);
  // server context mock
  Mock<IServerContext> server_ctx_mock;
  When(Method(server_ctx_mock, SearchLocation))
      .AlwaysReturn(Result<LocationContext, std::string>(Ok(ctx)));
  When(Method(server_ctx_mock, GetRoot)).AlwaysReturn(root);
  When(Method(server_ctx_mock, GetIndex)).AlwaysReturn(uri.substr(1));

  // config mock
  Mock<IConfig> config_mock;
  When(Method(config_mock, SearchServer)).AlwaysReturn(server_ctx_mock.get());

  HTTPRequest request;
  request.SetUri(uri);
  response = RequestHandler::Get(config_mock.get(), &request, "80", "");

  ASSERT_EQ(response->GetStatusCode(), http::kOk);
  ASSERT_EQ(response->GetBody(), "Hello, world!\n");

  unlink(test_file_path.c_str());
}

TEST_F(RequestHandlerTest, PostMethodNormal) {
  const std::string root = "/tmp";
  const std::string uri = "/index.html";
  const std::string body = "Hello, world!\n";

  LocationContext ctx;
  ctx.SetCanAutoIndex(true);
  Mock<IServerContext> server_ctx_mock;
  When(Method(server_ctx_mock, SearchLocation))
      .AlwaysReturn(Result<LocationContext, std::string>(Ok(ctx)));
  When(Method(server_ctx_mock, GetRoot)).AlwaysReturn(root);
  When(Method(server_ctx_mock, GetIndex)).AlwaysReturn(uri.substr(1));

  // config mock
  Mock<IConfig> config_mock;
  When(Method(config_mock, SearchServer)).AlwaysReturn(server_ctx_mock.get());

  HTTPRequest request;
  request.SetUri(uri);
  request.AddBody(body);
  response = RequestHandler::Post(config_mock.get(), &request, "80", "");

  ASSERT_EQ(response->GetStatusCode(), http::kCreated);
  ASSERT_EQ(response->GetHeaders().at("Location"), uri);

  std::ifstream ifs(test_file_path);
  // read all data from file
  std::string created_body((std::istreambuf_iterator<char>(ifs)),
                           (std::istreambuf_iterator<char>()));

  ASSERT_STRCASEEQ(body.c_str(), created_body.c_str());
}

TEST_F(RequestHandlerTest, PostMethodDirectoryRequest) {
  const std::string root = "/tmp";
  const std::string uri = "/";

  LocationContext ctx;
  ctx.SetCanAutoIndex(true);
  Mock<IServerContext> mock;
  When(Method(mock, SearchLocation))
      .AlwaysReturn(Result<LocationContext, std::string>(Ok(ctx)));
  When(Method(mock, GetRoot)).AlwaysReturn(root);
  When(Method(mock, GetIndex)).AlwaysReturn(uri.substr(1));

  HTTPRequest request;
  request.SetUri(uri);
  response = RequestHandler::Post(mock.get(), &request);

  ASSERT_EQ(response->GetStatusCode(), http::kBadRequest);
}
