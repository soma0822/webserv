#!/usr/bin/env python3

import cgi
import cgitb
cgitb.enable()

print("Content-type: text/html; charset=utf-8\r\n\r\n", end="")
print("<html><head><title>サーバーテスト</title><meta http-equiv=\"content-type\" charset=\"utf-8\"><link rel=\"stylesheet\" href=\"/styles.css\"></head><body><header><nav><a href=\"/\">ホーム</a></nav></header>")

form = cgi.FieldStorage()

for key in form:
    value = form[key].value
    print('<p>%s: %s</p>' % (key, value))

print("</body></html>")

