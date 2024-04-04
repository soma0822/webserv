#!/usr/bin/env python3

from datetime import datetime

# 現在の時刻を取得
now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

# HTMLの文字列を作成
html_content = f"""
<!DOCTYPE html>
<html lang="ja">
<head>
    <meta charset="UTF-8">
    <link rel=\"stylesheet\" href=\"/root.py/cgi-bin/time.css\">
    <title>現在時刻</title>
</head>
<body>
    <header><nav><a href="/">ホーム</a></nav></header>
    <div class="container">
        <h1>現在時刻: {now}</h1>
    </div>
        <button onclick="window.location.reload();">再確認</button>
    <div class="sidebar">
        <h2>Webserv</h2>
        <ul>
            <li><a href="https://www.google.com">Google</a></li>
            <li><a href="https://github.com/soma0822/webserv">Github</a></li>
        </ul>
    </div>
</body>
</html>
"""

# HTMLを出力
print("Content-type: text/html\r\n\r\n")
print(html_content)
print("\r\n")
