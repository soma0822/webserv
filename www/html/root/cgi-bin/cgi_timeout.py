#!/usr/bin/env python3

import cgi
import cgitb
import time
cgitb.enable()

print("Content-type: text/html; charset=utf-8\r\n\r\n", end="")
print("<html><head><title>サーバーテスト</title><meta http-equiv=\"content-type\" charset=\"utf-8\"><link rel=\"stylesheet\" href=\"/styles.css\"></head><body><header><nav><a href=\"/\">ホーム</a></nav></header>")
form = cgi.FieldStorage()

# GETメソッドで送信されたデータを取得
for key in form.keys():
    # FieldStorageオブジェクトのvalue属性を使用して値を取得
    value = form.getvalue(key)
    print('<p>%s: %s</p>' % (key, value))

print("</body></html>", flush=True)

while True:
    time.sleep(0.01)
