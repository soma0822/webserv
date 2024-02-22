#include <gtest/gtest.h>

#include "config.hpp"

TEST(ConfigTest, DefaultPath) {
  Config config;
  config.parse_file();
  ASSERT_EQ(config.get_server().size(), 1);
  ASSERT_EQ(config.get_server()[0].get_ip(), "127.0.0.1");
  ASSERT_EQ(config.get_server()[0].get_root(), "docs/fusion_web/");
  ASSERT_EQ(config.get_server()[0].get_index()[0], "index.html");
  ASSERT_EQ(config.get_server()[0].get_port()[0], "8002");
  ASSERT_EQ(config.get_server()[0].get_server_name()[0], "localhost");
  ASSERT_EQ(config.get_server()[0].get_error_page().size(), 2);
  std::map<std::string, std::string>::const_iterator it = config.get_server()[0].get_error_page().begin();
  ASSERT_EQ(it->first, "404"); 
  ASSERT_EQ(it->second, "error_pages/404.html"); 
  it++;
  ASSERT_EQ(it->first, "405"); 
  ASSERT_EQ(it->second, "error_pages/404.html");
  ASSERT_EQ(config.get_server()[0].get_location().size(), 4);
  std::map<std::string, LocationContext>::const_iterator it2 = config.get_server()[0].get_location().begin();
  ASSERT_EQ(it2->first, "/");
  ASSERT_EQ(it2->second.get_can_auto_index(), false);
  ASSERT_EQ(it2->second.get_limit_client_body(), 1000);
  ASSERT_EQ(it2->second.get_return(), "");
  ASSERT_EQ(it2->second.get_alias(), "");
  ASSERT_EQ(it2->second.get_root(), "");
  ASSERT_EQ(it2->second.get_index().size(), 0);
  ASSERT_EQ(it2->second.get_cgi_path().size(), 0);
  ASSERT_EQ(it2->second.get_cgi_extention().size(), 0);
  std::map<std::string, bool>::const_iterator it3 = it2->second.get_allow_method().begin();
  ASSERT_EQ(it3->second, true);
  it3++;
  ASSERT_EQ(it3->second, true);
  it3++;
  ASSERT_EQ(it3->second, true);
  ASSERT_EQ(it2->second.get_error_page().size(), 1);
  std::map<std::string, std::string>::const_iterator it4 = it2->second.get_error_page().begin();
  ASSERT_EQ(it4->first, "404");
  ASSERT_EQ(it4->second, "error_pages/404.html");
  it2++;
  ASSERT_EQ(it2->first, "/cgi-bin");
  ASSERT_EQ(it2->second.get_can_auto_index(), false);
  ASSERT_EQ(it2->second.get_limit_client_body(), INT_MAX);
  ASSERT_EQ(it2->second.get_return(), "");
  ASSERT_EQ(it2->second.get_alias(), "");
  ASSERT_EQ(it2->second.get_root(), "./");
  ASSERT_EQ(it2->second.get_index().size(), 1);
  ASSERT_EQ(it2->second.get_index()[0], "time.py");
  ASSERT_EQ(it2->second.get_cgi_path().size(), 2);
  ASSERT_EQ(it2->second.get_cgi_path()[0], "/usr/bin/python3");
  ASSERT_EQ(it2->second.get_cgi_path()[1], "/bin/bash");
  ASSERT_EQ(it2->second.get_cgi_extention().size(), 2);
  ASSERT_EQ(it2->second.get_cgi_extention()[0], ".py");
  ASSERT_EQ(it2->second.get_cgi_extention()[1], ".sh");
  std::map<std::string, bool>::const_iterator it5 = it2->second.get_allow_method().begin();
  ASSERT_EQ(it5->second, true);
  it5++;
  ASSERT_EQ(it5->second, true);
  it5++;
  ASSERT_EQ(it5->second, true);
  ASSERT_EQ(it2->second.get_error_page().size(), 0);
  it2++;
  ASSERT_EQ(it2->first, "/red");
  ASSERT_EQ(it2->second.get_can_auto_index(), true);
  ASSERT_EQ(it2->second.get_limit_client_body(), INT_MAX);
  ASSERT_EQ(it2->second.get_return(), "/tours");
  ASSERT_EQ(it2->second.get_alias(), "");
  ASSERT_EQ(it2->second.get_root(), "");
  ASSERT_EQ(it2->second.get_index().size(), 0);
  ASSERT_EQ(it2->second.get_cgi_path().size(), 0);
  ASSERT_EQ(it2->second.get_cgi_extention().size(), 0);
  std::map<std::string, bool>::const_iterator it6 = it2->second.get_allow_method().begin();
  ASSERT_EQ(it6->second, false);
  it6++;
  ASSERT_EQ(it6->second, false);
  it6++;
  ASSERT_EQ(it6->second, false);
  ASSERT_EQ(it2->second.get_error_page().size(), 0);
  it2++;
  ASSERT_EQ(it2->first, "/tours");
  ASSERT_EQ(it2->second.get_can_auto_index(), false);
  ASSERT_EQ(it2->second.get_limit_client_body(), INT_MAX);
  ASSERT_EQ(it2->second.get_return(), "");
  ASSERT_EQ(it2->second.get_alias(), "");
  ASSERT_EQ(it2->second.get_root(), "");
  ASSERT_EQ(it2->second.get_index().size(), 1);
  ASSERT_EQ(it2->second.get_index()[0], "tours1.html");
  ASSERT_EQ(it2->second.get_cgi_path().size(), 0);
  ASSERT_EQ(it2->second.get_cgi_extention().size(), 0);
  std::map<std::string, bool>::const_iterator it7 = it2->second.get_allow_method().begin();
  ASSERT_EQ(it7->second, false);
  it7++;
  ASSERT_EQ(it7->second, true);
  it7++;
  ASSERT_EQ(it7->second, true);
  ASSERT_EQ(it2->second.get_error_page().size(), 0);
  it2++;
  ASSERT_EQ(it2, config.get_server()[0].get_location().end());
}

TEST(ConfigTest, PortError){
  Config config("conf/port_error.conf");
  ASSERT_THROW(config.parse_file(), std::exception);
}

TEST(ConfigTest, ErrorPageError){
  Config config("conf/error_page_error.conf");
  ASSERT_THROW(config.parse_file(), std::exception);
}

TEST(ConfigTest, InvalidKeyTest){
  Config config("conf/invalid_key.conf");
  ASSERT_THROW(config.parse_file(), std::exception);
}