#!/usr/bin/env python3

import cgi
import cgitb
cgitb.enable()

print("Content-type: text/html; charset=utf-8\r\n\r\n", end="")
print("<html><body>")
form = cgi.FieldStorage()

# GETメソッドで送信されたデータを取得
for key in form.keys():
    # FieldStorageオブジェクトのvalue属性を使用して値を取得
    value = form.getvalue(key)
    print('<p>%s: %s</p>' % (key, value))

print("</body></html>")
