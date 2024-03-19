#!/usr/bin/env python

import sys
import urllib.parse
import os

# クエリ文字列を取得
query_string = os.environ.get('QUERY_STRING', '')

# クエリ文字列をフォームデータに解析
form_data = urllib.parse.parse_qs(query_string)

# Content-Type ヘッダーを出力
# フォームデータを出力
print("<meta Content-Type: text/html; charset=UTF-8>")
print("<html><body>")
for key, value in form_data.items():
    # 文字列を正しくエンコードして出力
    print(f"<p>{key}: {value[0]}</p>")
print("</body></html>")
