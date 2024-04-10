#!/usr/bin/env ruby

require 'cgi'

cgi = CGI.new

cgi.out("content-type" => "text/html") do
  "<html><head><title>CGI Test</title></head><body>" +
  "<h1>CGI Test Page</h1>" +
  "<p>Query Parameters:</p>" +
  "<ul>" +
  cgi.params.map { |key, value|
    "<li>#{key}: #{value.join(', ')}</li>"
  }.join('') +
  "</ul>" +
  "</body></html>"
end