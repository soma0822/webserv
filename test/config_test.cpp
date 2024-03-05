#include "config.hpp"

#include <gtest/gtest.h>

#include "config_parser.hpp"
#include "server_context.hpp"

TEST(ConfigTest, DefaultPath) {
  ConfigParser::Parse("test/conf_test/default.conf");
  ASSERT_EQ(Config::GetServer().size(), 1);
  ASSERT_EQ(Config::GetServer()[0].GetIp(), "127.0.0.1");
  ASSERT_EQ(Config::GetServer()[0].GetRoot(), "docs/fusion_web/");
  ASSERT_EQ(Config::GetServer()[0].GetIndex()[0], "index.html");
  ASSERT_EQ(Config::GetServer()[0].GetPort()[0], "8002");
  ASSERT_EQ(Config::GetServer()[0].GetServerName()[0], "localhost");
  ASSERT_EQ(Config::GetServer()[0].GetErrorPage().size(), 2);
  std::map<std::string, std::string>::const_iterator it =
      Config::GetServer()[0].GetErrorPage().begin();
  ASSERT_EQ(it->first, "404");
  ASSERT_EQ(it->second, "error_pages/404.html");
  ++it;
  ASSERT_EQ(it->first, "405");
  ASSERT_EQ(it->second, "error_pages/404.html");
  ASSERT_EQ(Config::GetServer()[0].GetLocation().size(), 4);
  std::map<std::string, LocationContext>::const_iterator it2 =
      Config::GetServer()[0].GetLocation().begin();
  ASSERT_EQ(it2->first, "/");
  ASSERT_EQ(it2->second.GetCnaAutoIndex(), false);
  ASSERT_EQ(it2->second.GetLimitClientBody(), 1000);
  ASSERT_EQ(it2->second.GetReturn(), "");
  ASSERT_EQ(it2->second.GetAlias(), "");
  ASSERT_EQ(it2->second.GetRoot(), "");
  ASSERT_EQ(it2->second.GetIndex().size(), 0);
  ASSERT_EQ(it2->second.GetCgiPath().size(), 0);
  ASSERT_EQ(it2->second.GetCgiExtention().size(), 0);
  std::map<std::string, bool>::const_iterator it3 =
      it2->second.GetAllowMethod().begin();
  ASSERT_EQ(it3->second, true);
  ++it3;
  ASSERT_EQ(it3->second, true);
  ++it3;
  ASSERT_EQ(it3->second, true);
  ASSERT_EQ(it2->second.GetErrorPage().size(), 1);
  std::map<std::string, std::string>::const_iterator it4 =
      it2->second.GetErrorPage().begin();
  ASSERT_EQ(it4->first, "404");
  ASSERT_EQ(it4->second, "error_pages/404.html");
  ++it2;
  ASSERT_EQ(it2->first, "/cgi-bin");
  ASSERT_EQ(it2->second.GetCnaAutoIndex(), false);
  ASSERT_EQ(it2->second.GetLimitClientBody(), 1000);
  ASSERT_EQ(it2->second.GetReturn(), "");
  ASSERT_EQ(it2->second.GetAlias(), "");
  ASSERT_EQ(it2->second.GetRoot(), "./");
  ASSERT_EQ(it2->second.GetIndex().size(), 1);
  ASSERT_EQ(it2->second.GetIndex()[0], "time.py");
  ASSERT_EQ(it2->second.GetCgiPath().size(), 2);
  ASSERT_EQ(it2->second.GetCgiPath()[0], "/usr/bin/python3");
  ASSERT_EQ(it2->second.GetCgiPath()[1], "/bin/bash");
  ASSERT_EQ(it2->second.GetCgiExtention().size(), 2);
  ASSERT_EQ(it2->second.GetCgiExtention()[0], ".py");
  ASSERT_EQ(it2->second.GetCgiExtention()[1], ".sh");
  std::map<std::string, bool>::const_iterator it5 =
      it2->second.GetAllowMethod().begin();
  ASSERT_EQ(it5->second, true);
  ++it5;
  ASSERT_EQ(it5->second, true);
  ++it5;
  ASSERT_EQ(it5->second, true);
  ASSERT_EQ(it2->second.GetErrorPage().size(), 0);
  ++it2;
  ASSERT_EQ(it2->first, "/red");
  ASSERT_EQ(it2->second.GetCnaAutoIndex(), true);
  ASSERT_EQ(it2->second.GetLimitClientBody(), 1000);
  ASSERT_EQ(it2->second.GetReturn(), "/tours");
  ASSERT_EQ(it2->second.GetAlias(), "");
  ASSERT_EQ(it2->second.GetRoot(), "");
  ASSERT_EQ(it2->second.GetIndex().size(), 0);
  ASSERT_EQ(it2->second.GetCgiPath().size(), 0);
  ASSERT_EQ(it2->second.GetCgiExtention().size(), 0);
  std::map<std::string, bool>::const_iterator it6 =
      it2->second.GetAllowMethod().begin();
  ASSERT_EQ(it6->second, false);
  ++it6;
  ASSERT_EQ(it6->second, false);
  ++it6;
  ASSERT_EQ(it6->second, false);
  ASSERT_EQ(it2->second.GetErrorPage().size(), 0);
  ++it2;
  ASSERT_EQ(it2->first, "/tours");
  ASSERT_EQ(it2->second.GetCnaAutoIndex(), false);
  ASSERT_EQ(it2->second.GetLimitClientBody(), 1000);
  ASSERT_EQ(it2->second.GetReturn(), "");
  ASSERT_EQ(it2->second.GetAlias(), "");
  ASSERT_EQ(it2->second.GetRoot(), "");
  ASSERT_EQ(it2->second.GetIndex().size(), 1);
  ASSERT_EQ(it2->second.GetIndex()[0], "tours1.html");
  ASSERT_EQ(it2->second.GetCgiPath().size(), 0);
  ASSERT_EQ(it2->second.GetCgiExtention().size(), 0);
  std::map<std::string, bool>::const_iterator it7 =
      it2->second.GetAllowMethod().begin();
  ASSERT_EQ(it7->second, false);
  ++it7;
  ASSERT_EQ(it7->second, true);
  ++it7;
  ASSERT_EQ(it7->second, true);
  ASSERT_EQ(it2->second.GetErrorPage().size(), 0);
  ++it2;
  ASSERT_EQ(it2, Config::GetServer()[0].GetLocation().end());
}

TEST(ConfigTest, TooLargePortTest) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/too_large_port.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, NotNumberPortTest) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/not_num_port.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, InvalidServerTest) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/invalid_server.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, NoFileTest) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/no_file.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, InvalidLocationKeyTest) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/invalid_location_key.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, InvalidLocationValueTest) {
  ASSERT_THROW(
      ConfigParser::Parse("test/conf_test/invalid_location_value.conf"),
      std::invalid_argument);
}

TEST(ConfigTest, LocationSyntaxErrorTest) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/location_syntax_error.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, ServerSyntaxErrorTest) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/server_syntax_error.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, InvalidServerKeyTest) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/invalid_server_key.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, InvalidServerValueTest) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/invalid_server_value.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, NoPortTest) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/no_port.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, DoubleIp) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/double_ip.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, DoubleRoot) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/double_root.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, DoubleIndex) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/double_index.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, DoublePort) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/double_port.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, DoubleServerName) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/double_server_name.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, DoublePair) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/double_server_pair.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, DoubleErrorPage) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/double_error_page.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, DoubleLocation) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/double_location.conf"),
               std::invalid_argument);
}
// location double
TEST(ConfigTest, DoubleLocationCanAutoIndex) {
  ASSERT_THROW(
      ConfigParser::Parse("test/conf_test/double_location_can_auto_index.conf"),
      std::invalid_argument);
}

TEST(ConfigTest, DoubleLocationLimitClientBody) {
  ASSERT_THROW(ConfigParser::Parse(
                   "test/conf_test/double_location_limit_client_body.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, DoubleLocationReturn) {
  ASSERT_THROW(
      ConfigParser::Parse("test/conf_test/double_location_return.conf"),
      std::invalid_argument);
}

TEST(ConfigTest, DoubleLocationalias) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/double_location_alias.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, DoubleLocationRoot) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/double_location_root.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, DoubleLocationIndex) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/double_location_index.conf"),
               std::invalid_argument);
}
TEST(ConfigTest, DoubleLocationCgi_Path) {
  ASSERT_THROW(
      ConfigParser::Parse("test/conf_test/double_location_cgi_path.conf"),
      std::invalid_argument);
}
TEST(ConfigTest, DoubleLocationCgiExtention) {
  ASSERT_THROW(
      ConfigParser::Parse("test/conf_test/double_location_Cgi_Extention.conf"),
      std::invalid_argument);
}

TEST(ConfigTest, DoubleLocationAllowMethod) {
  ASSERT_THROW(
      ConfigParser::Parse("test/conf_test/double_location_allow_method.conf"),
      std::invalid_argument);
}

TEST(ConfigTest, DoubleLocationErrorPage) {
  ASSERT_THROW(
      ConfigParser::Parse("test/conf_test/double_location_error_page.conf"),
      std::invalid_argument);
}

// SerchServer
TEST(SerchServer, DefaultTest) {
  ConfigParser::Parse("test/conf_test/search_server.conf");
  const ServerContext &tmp = Config::SearchServer("8002", "");
  ASSERT_EQ(&Config::GetServer().at(0), &tmp);
  const ServerContext &tmp1 = Config::SearchServer("8000", "");
  ASSERT_EQ(&Config::GetServer().at(1), &tmp1);
  const ServerContext &tmp2 = Config::SearchServer("8000", "tokazaki");
  ASSERT_EQ(&Config::GetServer().at(1), &tmp2);
  const ServerContext &tmp3 = Config::SearchServer("8002", "tkuramot");
  ASSERT_EQ(&Config::GetServer().at(2), &tmp3);
}
