#!/usr/bin/env python3

# -*- coding: utf-8 -*-

from http import cookies
import os
import sys
import datetime

# 関数定義：クッキーの取得
def get_cookie():
    cookie = cookies.SimpleCookie(os.environ.get('HTTP_COOKIE'))
    return cookie.get('visit_count')

# メイン処理
def main():
    string = "status: 200 ok\n"
    # HTMLヘッダーの出力
    string += "Content-Type: text/html; charset=utf-8\n"
    string += "Content-Length: 577\n"

    # クッキーの取得
    visit_cookie = get_cookie()
    if visit_cookie:
        # クッキーがある場合、訪問回数を更新
        n = int(visit_cookie.value) + 1
    else:
        # クッキーがない場合、初めての訪問
        n = 1

    # クッキーの設定
    cookie = cookies.SimpleCookie()
    cookie['visit_count'] = str(n)
    cookie['visit_count']['expires'] = (datetime.datetime.now() + datetime.timedelta(days=1)).strftime("%a, %d-%b-%Y %H:%M:%S GMT")
    string += cookie.output()
    string += "\n"

    # HTMLの出力
    string += """
<!DOCTYPE html>
<html>
<head>
    <title>Pythonでcookieを使った訪問回数の記録方法</title>
</head>
<body>
    <center>
        <table border='1' width='90%' bgcolor='#FDF5E6'>
            <th>このページの訪問回数</th>
            <tr>
                <td>
                    {} 回訪問
                </td>
            </tr>
        </table>
    </center>
</body>
</html>
""".format(n)
    string += "\n"
    string += "\n"
    return string

if __name__ == "__main__":
    string = main()
    sys.stderr.write(string)
    print(string)
