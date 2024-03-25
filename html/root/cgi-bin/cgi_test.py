#!/usr/bin/python3

import cgi
import cgitb
cgitb.enable()

print("Content-type: text/html; charset=utf-8\r\n\r\n", end="")
print("<html><body>")
form = cgi.FieldStorage()

for key in form:
    value = form[key].value
    print('<p>%s: %s</p>' % (key, value))

print("</body></html>")

