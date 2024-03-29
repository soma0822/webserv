#include "config.hpp"

#include <gtest/gtest.h>

#include "config_parser.hpp"
#include "server_context.hpp"

TEST(ConfigTest, EmptyConfig) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/empty.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, DefaultPath) {
  Config config = ConfigParser::Parse("test/conf_test/default.conf");
  std::map<std::string, std::string>::const_iterator it =
      config.GetErrorPage().begin();
  ASSERT_EQ(it->first, "404");
  ASSERT_EQ(it->second, "error_pages/404.html");
  ++it;
  ASSERT_EQ(it->first, "405");
  ASSERT_EQ(it->second, "error_pages/404.html");
  ASSERT_EQ(config.GetServer().size(), 1);
  ASSERT_EQ(config.GetServer()[0].GetIp(), "127.0.0.1");
  ASSERT_EQ(config.GetServer()[0].GetRoot(), "docs/fusion_web/");
  ASSERT_EQ(config.GetServer()[0].GetIndex(), "index.html");
  ASSERT_EQ(config.GetServer()[0].GetPort(), "8002");
  ASSERT_EQ(config.GetServer()[0].GetServerName(), "localhost");
  ASSERT_EQ(config.GetServer()[0].GetLocation().size(), 6);
  std::map<std::string, LocationContext>::const_iterator it2 =
      config.GetServer()[0].GetLocation().begin();
  it2++;
  ASSERT_EQ(it2->first, "/");
  ASSERT_EQ(it2->second.GetCanAutoIndex(), false);
  ASSERT_EQ(it2->second.GetLimitClientBody(), 1000);
  ASSERT_EQ(it2->second.GetReturn(), "");
  ASSERT_EQ(it2->second.GetAlias(), "");
  ASSERT_EQ(it2->second.GetRoot(), "");
  ASSERT_EQ(it2->second.GetIndex(), "");
  ASSERT_EQ(it2->second.GetCgiExtension().size(), 0);
  std::map<std::string, bool>::const_iterator it3 =
      it2->second.GetAllowMethod().begin();
  ASSERT_EQ(it3->second, true);
  ++it3;
  ASSERT_EQ(it3->second, true);
  ++it3;
  ASSERT_EQ(it3->second, true);
  ++it2;
  ASSERT_EQ(it2->first, "/cgi-bin");
  ASSERT_EQ(it2->second.GetCanAutoIndex(), false);
  ASSERT_EQ(it2->second.GetLimitClientBody(), 1000);
  // ASSERT_EQ(it2->second.GetPath(), "/cgi-bin");
  ASSERT_EQ(it2->second.GetReturn(), "");
  ASSERT_EQ(it2->second.GetAlias(), "");
  ASSERT_EQ(it2->second.GetRoot(), "./");
  ASSERT_EQ(it2->second.GetIndex(), "time.py");
  ASSERT_EQ(it2->second.GetCgiExtension().size(), 2);
  ASSERT_EQ(it2->second.GetCgiExtension()[0], ".py");
  ASSERT_EQ(it2->second.GetCgiExtension()[1], ".sh");
  std::map<std::string, bool>::const_iterator it5 =
      it2->second.GetAllowMethod().begin();
  ASSERT_EQ(it5->second, true);
  ++it5;
  ASSERT_EQ(it5->second, true);
  ++it5;
  ASSERT_EQ(it5->second, true);
  ++it2;
  ASSERT_EQ(it2->first, "/red");
  ASSERT_EQ(it2->second.GetCanAutoIndex(), true);
  ASSERT_EQ(it2->second.GetLimitClientBody(), 1000);
  ASSERT_EQ(it2->second.GetReturn(), "/tours");
  ASSERT_EQ(it2->second.GetAlias(), "");
  ASSERT_EQ(it2->second.GetRoot(), "");
  ASSERT_EQ(it2->second.GetIndex(), "");
  ASSERT_EQ(it2->second.GetCgiExtension().size(), 0);
  std::map<std::string, bool>::const_iterator it6 =
      it2->second.GetAllowMethod().begin();
  ASSERT_EQ(it6->second, false);
  ++it6;
  ASSERT_EQ(it6->second, true);
  ++it6;
  ASSERT_EQ(it6->second, false);
  ++it2;
  ASSERT_EQ(it2->first, "/tours");
  ASSERT_EQ(it2->second.GetCanAutoIndex(), false);
  ASSERT_EQ(it2->second.GetLimitClientBody(), 1000);
  ASSERT_EQ(it2->second.GetReturn(), "");
  ASSERT_EQ(it2->second.GetAlias(), "");
  ASSERT_EQ(it2->second.GetRoot(), "");
  ASSERT_EQ(it2->second.GetIndex(), "tours1.html");
  ASSERT_EQ(it2->second.GetCgiExtension().size(), 0);
  std::map<std::string, bool>::const_iterator it7 =
      it2->second.GetAllowMethod().begin();
  ASSERT_EQ(it7->second, false);
  ++it7;
  ASSERT_EQ(it7->second, true);
  ++it7;
  ASSERT_EQ(it7->second, true);
  ++it2;
  ASSERT_EQ(it2->first, "= /red");
  ASSERT_EQ(it2->second.GetPath(), "= /red");
  ++it2;
  ASSERT_EQ(it2, config.GetServer()[0].GetLocation().end());
  ASSERT_EQ("/", config.GetServer()[0].SearchLocation("/").GetPath());
  ASSERT_EQ(
      "/",
      config.GetServer()[0].SearchLocation("/index.html").GetPath());
  ASSERT_EQ("= /red",
            config.GetServer()[0].SearchLocation("/red").GetPath());
  ASSERT_EQ("/red",
            config.GetServer()[0].SearchLocation("/red/").GetPath());
  ASSERT_EQ("/tours", config.GetServer()[0]
                          .SearchLocation("/tours/index.html")
                          .GetPath());
  ASSERT_EQ("/tours",
            config.GetServer()[0].SearchLocation("/tours/").GetPath());
  ASSERT_EQ("/tours",
            config.GetServer()[0].SearchLocation("/tours").GetPath());
}

TEST(ConfigTest, NoRootTest) {
  const Config config = ConfigParser::Parse("test/conf_test/no_root.conf");
  ASSERT_EQ(config.GetServer().at(0).GetRoot(), "./www/html");
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
TEST(ConfigTest, DoubleLocationCgiExtension) {
  ASSERT_THROW(
      ConfigParser::Parse("test/conf_test/double_location_Cgi_Extension.conf"),
      std::invalid_argument);
}

TEST(ConfigTest, DoubleLocationAllowMethod) {
  ASSERT_THROW(
      ConfigParser::Parse("test/conf_test/double_location_allow_method.conf"),
      std::invalid_argument);
}

TEST(ConfigTest, PortIp) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/port_ip.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, PortIpServerName) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/port_ip_server_name.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, HostListen) {
  ASSERT_THROW(ConfigParser::Parse("test/conf_test/host_listen.conf"),
               std::invalid_argument);
}

TEST(ConfigTest, HostListenServername) {
  ASSERT_THROW(
      ConfigParser::Parse("test/conf_test/host_listen_server_name.conf"),
      std::invalid_argument);
}

// SerchServer
TEST(SerchServer, DefaultTest) {
  Config config = ConfigParser::Parse("test/conf_test/search_server.conf");
  const IServerContext &tmp = config.SearchServer("8002", "127.0.0.1", "");
  ASSERT_EQ(&config.GetServer().at(0), &tmp);
  const IServerContext &tmp1 = config.SearchServer("8000", "127.0.0.1", "");
  ASSERT_EQ(&config.GetServer().at(1), &tmp1);
  const IServerContext &tmp2 =
      config.SearchServer("8000", "127.0.0.1", "tokazaki");
  ASSERT_EQ(&config.GetServer().at(1), &tmp2);
  const IServerContext &tmp3 =
      config.SearchServer("8002", "127.0.0.1", "tkuramot");
  ASSERT_EQ(&config.GetServer().at(2), &tmp3);
}

// SerchLocation
TEST(SerchLocation, DefaultTest) {
  Config config = ConfigParser::Parse("test/conf_test/search_location.conf");
  const IServerContext &tmp =
      config.SearchServer("8002", "127.0.0.1", "localhost");
  ASSERT_EQ("/", tmp.SearchLocation("/").GetPath());
  ASSERT_EQ("/red", tmp.SearchLocation("/red/bin").GetPath());
  ASSERT_EQ("= /red", tmp.SearchLocation("/red").GetPath());
  ASSERT_EQ("/tours", tmp.SearchLocation("/tours/usrs").GetPath());
  ASSERT_EQ("/tours/usr",
            tmp.SearchLocation("/tours/usr/sina").GetPath());
  ASSERT_EQ("",
            tmp.SearchLocation("").GetPath());
}
