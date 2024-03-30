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
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>現在時刻</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background-color: #f2f2f2; /* 薄いグレーの背景色 */
            display: flex;
            flex-direction: column;
            min-height: 100vh; /* 画面の高さいっぱいにする */
        }}
        .container {{
            margin: 20px auto; /* メインコンテンツの上部マージン */
            border: 2px solid #333;
            border-radius: 10px;
            padding: 20px;
            background-color: #fff;
            margin-bottom: 20px; /* メインコンテンツとの隙間 */
        }}
        h1 {{
            font-size: 24px;
            color: #333;
        }}
        .sidebar {{
            position: fixed;
            bottom: 0;
            width: 100%;
            background-color: #333;
            color: #fff; /* テキストの色 */
            padding: 20px;
            text-align: center;
        }}
        a, button {{
            color: #808080;
            text-decoration: none;
            padding: 10px 20px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            margin-right: 10px;
        }}
        a:hover, button:hover {{
            background-color: #f7e78e;
            padding: 8px 16px; /* ボタンの余白を調整 */
        }}
        .sidebar h2 {{
        text-align: left; /* タイトルとリストを左揃えに */
        }}
        .sidebar ul {{
            list-style-type: none; /* リストのマーカー（点）をなくす */
            padding: 0; /* リストのパディングをリセット */
            margin: 0; /* リストのマージンをリセット */
        }}
        .sidebar li {{
        text-align: left; /* タイトルとリストを左揃えに */
        }}
    </style>
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
            <li><a href="https://www.google.com">Googleに移動</a></li>
            <li>項目2</li>
            <li>項目3</li>
        </ul>
    </div>
</body>
</html>
"""

# HTMLを出力
print("Content-type: text/html\r\n\r\n")
print(html_content)
print("\r\n")
