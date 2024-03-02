#include <iostream>

#include "http_request_parser.hpp"

HTTPRequestParser::HTTPRequestParser() {}
HTTPRequestParser::~HTTPRequestParser() {}
HTTPRequestParser::HTTPRequestParser(const HTTPRequestParser &other) {}
HTTPRequestParser &HTTPRequestParser::operator=(const HTTPRequestParser &other) {}

const HTTPRequest *HTTPRequestParser::GetRequestInstance(std::string request_line, std::string port) {
	try {
	HTTPRequest *request = new HTTPRequest();
	return request;
	} catch (std::exception &e) {
		std::cout << "Error: " << e.what() << std::endl;
		return NULL;
	}
}
