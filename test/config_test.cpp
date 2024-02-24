#include <gtest/gtest.h>

#include "config.hpp"

TEST(ConfigTest, DefaultPath) {
  Config config("test/conf_test/default.conf");
  config.ParseFile();
  ASSERT_EQ(config.GetServer().size(), 1);
  ASSERT_EQ(config.GetServer()[0].GetIp(), "127.0.0.1");
  ASSERT_EQ(config.GetServer()[0].GetRoot(), "docs/fusion_web/");
  ASSERT_EQ(config.GetServer()[0].GetIndex()[0], "index.html");
  ASSERT_EQ(config.GetServer()[0].GetPort()[0], "8002");
  ASSERT_EQ(config.GetServer()[0].GetServerName()[0], "localhost");
  ASSERT_EQ(config.GetServer()[0].GetErrorPage().size(), 2);
  std::map<std::string, std::string>::const_iterator it = config.GetServer()[0].GetErrorPage().begin();
  ASSERT_EQ(it->first, "404"); 
  ASSERT_EQ(it->second, "error_pages/404.html"); 
  it++;
  ASSERT_EQ(it->first, "405"); 
  ASSERT_EQ(it->second, "error_pages/404.html");
  ASSERT_EQ(config.GetServer()[0].GetLocation().size(), 4);
  std::map<std::string, LocationContext>::const_iterator it2 = config.GetServer()[0].GetLocation().begin();
  ASSERT_EQ(it2->first, "/");
  ASSERT_EQ(it2->second.GetCnaAutoIndex(), false);
  ASSERT_EQ(it2->second.GetLimitClientBody(), 1000);
  ASSERT_EQ(it2->second.GetReturn(), "");
  ASSERT_EQ(it2->second.GetAlias(), "");
  ASSERT_EQ(it2->second.GetRoot(), "");
  ASSERT_EQ(it2->second.GetIndex().size(), 0);
  ASSERT_EQ(it2->second.GetCgiPath().size(), 0);
  ASSERT_EQ(it2->second.GetCgiExtention().size(), 0);
  std::map<std::string, bool>::const_iterator it3 = it2->second.GetAllowMethod().begin();
  ASSERT_EQ(it3->second, true);
  it3++;
  ASSERT_EQ(it3->second, true);
  it3++;
  ASSERT_EQ(it3->second, true);
  ASSERT_EQ(it2->second.GetErrorPage().size(), 1);
  std::map<std::string, std::string>::const_iterator it4 = it2->second.GetErrorPage().begin();
  ASSERT_EQ(it4->first, "404");
  ASSERT_EQ(it4->second, "error_pages/404.html");
  it2++;
  ASSERT_EQ(it2->first, "/cgi-bin");
  ASSERT_EQ(it2->second.GetCnaAutoIndex(), false);
  ASSERT_EQ(it2->second.GetLimitClientBody(), 2147483647);
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
  std::map<std::string, bool>::const_iterator it5 = it2->second.GetAllowMethod().begin();
  ASSERT_EQ(it5->second, true);
  it5++;
  ASSERT_EQ(it5->second, true);
  it5++;
  ASSERT_EQ(it5->second, true);
  ASSERT_EQ(it2->second.GetErrorPage().size(), 0);
  it2++;
  ASSERT_EQ(it2->first, "/red");
  ASSERT_EQ(it2->second.GetCnaAutoIndex(), true);
  ASSERT_EQ(it2->second.GetLimitClientBody(), 2147483647);
  ASSERT_EQ(it2->second.GetReturn(), "/tours");
  ASSERT_EQ(it2->second.GetAlias(), "");
  ASSERT_EQ(it2->second.GetRoot(), "");
  ASSERT_EQ(it2->second.GetIndex().size(), 0);
  ASSERT_EQ(it2->second.GetCgiPath().size(), 0);
  ASSERT_EQ(it2->second.GetCgiExtention().size(), 0);
  std::map<std::string, bool>::const_iterator it6 = it2->second.GetAllowMethod().begin();
  ASSERT_EQ(it6->second, false);
  it6++;
  ASSERT_EQ(it6->second, false);
  it6++;
  ASSERT_EQ(it6->second, false);
  ASSERT_EQ(it2->second.GetErrorPage().size(), 0);
  it2++;
  ASSERT_EQ(it2->first, "/tours");
  ASSERT_EQ(it2->second.GetCnaAutoIndex(), false);
  ASSERT_EQ(it2->second.GetLimitClientBody(), 2147483647);
  ASSERT_EQ(it2->second.GetReturn(), "");
  ASSERT_EQ(it2->second.GetAlias(), "");
  ASSERT_EQ(it2->second.GetRoot(), "");
  ASSERT_EQ(it2->second.GetIndex().size(), 1);
  ASSERT_EQ(it2->second.GetIndex()[0], "tours1.html");
  ASSERT_EQ(it2->second.GetCgiPath().size(), 0);
  ASSERT_EQ(it2->second.GetCgiExtention().size(), 0);
  std::map<std::string, bool>::const_iterator it7 = it2->second.GetAllowMethod().begin();
  ASSERT_EQ(it7->second, false);
  it7++;
  ASSERT_EQ(it7->second, true);
  it7++;
  ASSERT_EQ(it7->second, true);
  ASSERT_EQ(it2->second.GetErrorPage().size(), 0);
  it2++;
  ASSERT_EQ(it2, config.GetServer()[0].GetLocation().end());
}

TEST(ConfigTest, TooLargePortTest){
  Config config("test/conf_test/too_large_port.conf");
  try {
    config.ParseFile();
  } catch (std::exception &e) {
    ASSERT_STREQ(e.what(), "20000000000000 is too large");
  }
}

TEST(ConfigTest, NotNumberPortTest){
  Config config("test/conf_test/not_num_port.conf");
  try {
    config.ParseFile();
  } catch (std::exception &e) {
    ASSERT_STREQ(e.what(), "200a is not a number");
  }
}

TEST(ConfigTest, InvalidServerTest){
  Config config("test/conf_test/invalid_server.conf");
  try {
    config.ParseFile();
  } catch (std::exception &e) {
    ASSERT_STREQ(e.what(), "Invalid key: servera {");
  }
}

TEST(ConfigTest, NoFileTest){
  Config config("test/conf_test/no_file.conf");
  try {
    config.ParseFile();
  } catch (std::exception &e) {
    ASSERT_STREQ(e.what(), "File could not open: test/conf_test/no_file.conf");
  }
}

TEST(ConfigTest, InvalidLocationKeyTest){
  Config config("test/conf_test/invalid_location_key.conf");
  try {
    config.ParseFile();
  } catch (std::exception &e) {
    ASSERT_STREQ(e.what(), "Invalid location key: tokazaki");
  }
}

TEST(ConfigTest, InvalidLocationValueTest){
  Config config("test/conf_test/invalid_location_value.conf");
  try {
    config.ParseFile();
  } catch (std::exception &e) {
    ASSERT_STREQ(e.what(), "Invalid location value: /usr/share/nginx/html /usr/share/nginx/html");
  }
}

TEST(ConfigTest, LocationSyntaxErrorTest){
  Config config("test/conf_test/location_syntax_error.conf");
  try {
    config.ParseFile();
  } catch (std::exception &e) {
    ASSERT_STREQ(e.what(), "Syntax error: semicolon: \t\troot /usr/share/nginx/html");
  }
}

TEST(ConfigTest, ServerSyntaxErrorTest){
  Config config("test/conf_test/server_syntax_error.conf");
  try {
    config.ParseFile();
  } catch (std::exception &e) {
    ASSERT_STREQ(e.what(), "Syntax error: 	location / ");
  }
}

TEST(ConfigTest, InvalidServerKeyTest){
  Config config("test/conf_test/invalid_server_key.conf");
  try {
    config.ParseFile();
  } catch (std::exception &e) {
    ASSERT_STREQ(e.what(), "Invalid server key: tkuramot");
  }
}

TEST(ConfigTest, InvalidServerValueTest){
  Config config("test/conf_test/invalid_server_value.conf");
  try {
    config.ParseFile();
  } catch (std::exception &e) {
    ASSERT_STREQ(e.what(), "Invalid server value: kuramoto okazaki");
  }
}
