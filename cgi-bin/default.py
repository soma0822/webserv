#!/usr/bin/env python

import cgi
import cgitb
cgitb.enable()

print("<meta charset=\"UTF-8\">")
print("<html><body>")
form = cgi.FieldStorage()

for key in form:
    value = form[key].value
    print('<p>%s: %s</p>' % (key, value))

print('<p>')
for i in range(100000):
    print('a', end='')
print('</p>')
print("</body></html>")
