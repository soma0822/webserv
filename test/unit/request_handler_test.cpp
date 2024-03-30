#include "request_handler.hpp"

#include <gtest/gtest.h>

#include <fakeit.hpp>
#include <filesystem>
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
  ctx.AddAllowMethod("GET");
  // server context mock
  Mock<IServerContext> server_ctx_mock;
  When(Method(server_ctx_mock, SearchLocation)).AlwaysReturn(ctx);
  When(Method(server_ctx_mock, GetRoot)).AlwaysReturn(root);
  When(Method(server_ctx_mock, GetIndex)).AlwaysReturn(uri.substr(1));

  // config mock
  Mock<IConfig> config_mock;
  When(Method(config_mock, SearchServer)).AlwaysReturn(server_ctx_mock.get());

  HTTPRequest request;
  request.SetUri(uri);
  request.SetMethod("GET");
  RequestContext req_ctx = {&request, "80", "", 0, 0};
  response = RequestHandler::Get(config_mock.get(), req_ctx).Unwrap();

  ASSERT_EQ(response->GetStatusCode(), http::kOk);
  ASSERT_EQ(response->GetBody(), "Hello, world!\n");

  unlink(test_file_path.c_str());
}

TEST_F(RequestHandlerTest, PostMethodNormal) {
  const std::string root = "/tmp";
  const std::string uri = "/index.html";
  const std::string index = "index.html";
  const std::string body = "Hello, world!\n";

  LocationContext ctx;
  ctx.SetCanAutoIndex(true);
  ctx.AddAllowMethod("POST");
  Mock<IServerContext> server_ctx_mock;
  When(Method(server_ctx_mock, SearchLocation)).AlwaysReturn(ctx);
  When(Method(server_ctx_mock, GetRoot)).AlwaysReturn(root);
  When(Method(server_ctx_mock, GetIndex)).AlwaysReturn(index);

  // config mock
  Mock<IConfig> config_mock;
  When(Method(config_mock, SearchServer)).AlwaysReturn(server_ctx_mock.get());

  HTTPRequest request;
  request.SetUri(uri);
  request.SetMethod("POST");
  request.AddBody(body);
  RequestContext req_ctx = {&request, "80", "", 0, 0};
  response = RequestHandler::Post(config_mock.get(), req_ctx).Unwrap();

  ASSERT_EQ(response->GetStatusCode(), http::kCreated);
  ASSERT_EQ(response->GetHeaders().at("LOCATION"), uri);

  std::ifstream ifs(test_file_path);
  // read all data from file
  std::string created_body((std::istreambuf_iterator<char>(ifs)),
                           (std::istreambuf_iterator<char>()));

  ASSERT_STRCASEEQ(body.c_str(), created_body.c_str());
}

TEST_F(RequestHandlerTest, PostMethodDirectoryRequest) {
  const std::string root = "/tmp";
  const std::string uri = "/";
  const std::string index = "";

  LocationContext ctx;
  ctx.SetCanAutoIndex(true);
  // server context mock
  Mock<IServerContext> server_ctx_mock;
  When(Method(server_ctx_mock, SearchLocation)).AlwaysReturn(ctx);
  When(Method(server_ctx_mock, GetRoot)).AlwaysReturn(root);
  When(Method(server_ctx_mock, GetIndex)).AlwaysReturn(index);

  // config mock
  Mock<IConfig> config_mock;
  std::map<std::string, std::string> error_page;
  When(Method(config_mock, SearchServer)).AlwaysReturn(server_ctx_mock.get());
  When(Method(config_mock, GetErrorPage)).AlwaysReturn(error_page);

  HTTPRequest request;
  request.SetUri(uri);
  RequestContext req_ctx = {&request, "80", "", 0, 0};
  response = RequestHandler::Post(config_mock.get(), req_ctx).Unwrap();

  ASSERT_EQ(response->GetStatusCode(), http::kBadRequest);
}

TEST_F(RequestHandlerTest, DeleteMethodNormalTest) {
  const std::string root = "/tmp";
  const std::string uri = "/index.html";

  std::ofstream ofs(test_file_path.c_str());
  ofs << "Hello, world!" << std::endl;
  ofs.close();

  LocationContext ctx;
  ctx.SetCanAutoIndex(true);
  ctx.AddAllowMethod("DELETE");
  // server context mock
  Mock<IServerContext> server_ctx_mock;
  When(Method(server_ctx_mock, SearchLocation)).AlwaysReturn(ctx);
  When(Method(server_ctx_mock, GetRoot)).AlwaysReturn(root);
  When(Method(server_ctx_mock, GetIndex)).AlwaysReturn(uri.substr(1));

  // config mock
  Mock<IConfig> config_mock;
  std::map<std::string, std::string> error_page;
  When(Method(config_mock, SearchServer)).AlwaysReturn(server_ctx_mock.get());
  When(Method(config_mock, GetErrorPage)).AlwaysReturn(error_page);

  HTTPRequest request;
  request.SetUri(uri);
  request.SetMethod("DELETE");
  RequestContext req_ctx = {&request, "80", "", 0, 0};
  response = RequestHandler::Delete(config_mock.get(), req_ctx).Unwrap();

  ASSERT_EQ(response->GetStatusCode(), http::kOk);
  const std::filesystem::path path(test_file_path);
  ASSERT_TRUE(!std::filesystem::exists(path));
}

TEST_F(RequestHandlerTest, DeleteMethodDirectoryTarget) {
  const std::string root = "/tmp";
  const std::string uri = "/";

  std::ofstream ofs(test_file_path.c_str());
  ofs << "Hello, world!" << std::endl;
  ofs.close();

  LocationContext ctx;
  ctx.SetCanAutoIndex(true);
  // server context mock
  Mock<IServerContext> server_ctx_mock;
  When(Method(server_ctx_mock, SearchLocation)).AlwaysReturn(ctx);
  When(Method(server_ctx_mock, GetRoot)).AlwaysReturn(root);
  When(Method(server_ctx_mock, GetIndex)).AlwaysReturn(uri.substr(1));

  // config mock
  Mock<IConfig> config_mock;
  std::map<std::string, std::string> error_page;
  When(Method(config_mock, SearchServer)).AlwaysReturn(server_ctx_mock.get());
  When(Method(config_mock, GetErrorPage)).AlwaysReturn(error_page);

  HTTPRequest request;
  request.SetUri(uri);
  RequestContext req_ctx = {&request, "80", "", 0, 0};
  response = RequestHandler::Delete(config_mock.get(), req_ctx).Unwrap();

  ASSERT_EQ(response->GetStatusCode(), http::kBadRequest);
  const std::filesystem::path path(test_file_path);
  ASSERT_TRUE(std::filesystem::exists(path));
}

// 存在するファイルが来たら、切るようにしてあるのでテストでも実際に存在するファイルにする必要があった。
TEST_F(RequestHandlerTest, ResolveRequestTargetPath) {
  const std::string root = "./www/html/root/cgi-bin";
  const std::string uri = "/cgi-bin/cgi_test.py/foo/bar";

  LocationContext ctx;
  ctx.SetCanAutoIndex(true);
  ctx.SetPath("/cgi-bin");
  ctx.AddCgiExtension(".py");
  // server context mock
  Mock<IServerContext> server_ctx_mock;
  When(Method(server_ctx_mock, SearchLocation)).AlwaysReturn(ctx);
  When(Method(server_ctx_mock, GetRoot)).AlwaysReturn(root);
  When(Method(server_ctx_mock, GetIndex)).AlwaysReturn(uri.substr(1));

  // config mock
  Mock<IConfig> config_mock;
  When(Method(config_mock, SearchServer)).AlwaysReturn(server_ctx_mock.get());

  HTTPRequest request;
  request.SetUri(uri);
  RequestContext req_ctx = {&request, "80", "", 0, 0};
  std::pair<std::string, std::string> path_pair =
      RequestHandler::ResolveRequestTargetPath(config_mock.get(), req_ctx);
  ASSERT_EQ(path_pair.first, "./www/html/root/cgi-bin/cgi_test.py");
  ASSERT_EQ(path_pair.second, "./www/html/root/cgi-bin/foo/bar");
}

TEST_F(RequestHandlerTest, ResolveRequestTargetPathSegmentWithoutPathSegment) {
  const std::string root = "./www/html/root/cgi-bin";
  const std::string uri = "/cgi-bin/cgi_test.py";

  LocationContext ctx;
  ctx.SetCanAutoIndex(true);
  ctx.SetPath("/cgi-bin");
  ctx.AddCgiExtension(".py");
  // server context mock
  Mock<IServerContext> server_ctx_mock;
  When(Method(server_ctx_mock, SearchLocation)).AlwaysReturn(ctx);
  When(Method(server_ctx_mock, GetRoot)).AlwaysReturn(root);
  When(Method(server_ctx_mock, GetIndex)).AlwaysReturn(uri.substr(1));

  // config mock
  Mock<IConfig> config_mock;
  When(Method(config_mock, SearchServer)).AlwaysReturn(server_ctx_mock.get());

  HTTPRequest request;
  request.SetUri(uri);
  RequestContext req_ctx = {&request, "80", "", 0, 0};
  std::pair<std::string, std::string> path_pair =
      RequestHandler::ResolveRequestTargetPath(config_mock.get(), req_ctx);
  ASSERT_EQ(path_pair.first, "./www/html/root/cgi-bin/cgi_test.py");
  ASSERT_EQ(path_pair.second, "");
}
