#!/usr/bin/env python

import sys
import urllib.parse

# リクエストボディを文字列としてデコード
body = sys.stdin.buffer.read().decode('utf-8')

# フォームデータとして解析
form_data = urllib.parse.parse_qs(body)

# Content-Type ヘッダーを出力
print()

print("<meta Content-Type: text/html; charset=UTF-8>")
# フォームデータを出力
print("<html><body>")
for key, value in form_data.items():
    print(f"<p>{key}: {value[0]}</p>")
print("</body></html>")


# print('<p>')
# for i in range(1000000):
#     print('a', end='')
# print('</p>')
# print("</body></html>")
