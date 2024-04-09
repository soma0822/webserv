#!/usr/bin/env python3

# -*- coding: utf-8 -*-

from http import cookies
import os
import sys
import datetime
import subprocess
from jinja2 import Template
import sqlite3
import fcntl
import hashlib
import time
import urllib.parse

# 関数定義：ファイルに情報を書き込む
def set_data(file_path, id_, name, value):
    try:
        with open(file_path, 'a') as file:
            fd = file.fileno()
            fcntl.flock(fd, fcntl.LOCK_EX)  # 排他ロック
            file.write(f"{id_},{name},{value}\n")
            fcntl.flock(fd, fcntl.LOCK_UN)  # ロック解除
    except Exception as e:
        sys.stderr.write(f"An error occurred: {e}")

# 関数定義：idをもとにファイルからのデータ取得
def get_data_from_id(file_path, target_id):
    try:
        with open(file_path, 'r') as file:
            for line in file:
                parts = line.strip().split(',',1)
                if target_id == parts[0]:
                    return "OK",parts[1]
            return "KO", ""
    except FileNotFoundError:
        print("File not found.")
        return None

# 関数定義：名前をもとにファイルからのデータ取得
def get_data_from_name(file_path, name):
    if not name:
        return "KO", ""
    name = urllib.parse.unquote(name)
    try:
        with open(file_path, 'r') as file:
            for line in file:
                parts = line.strip().split(',', 1)
                if len(parts) >= 2 and name == parts[1].split(',')[0]:
                    return "OK", parts
            return "Err", ""
    except FileNotFoundError:
        print("File not found.")
        return None

# 関数定義：ファイルからデータ削除
def delete_data(file_path, session_id):
    try:
        with open(file_path, 'r+') as file:
            fd = file.fileno()
            fcntl.flock(fd, fcntl.LOCK_EX)  # 排他ロック
            lines = file.readlines()
            file.seek(0)  # ファイルの先頭に移動
            file.truncate(0)  # ファイルの中身を空にする
            for line in lines:
                if session_id not in line:
                    file.write(line)
            fcntl.flock(fd, fcntl.LOCK_UN)  # ロック解除
    except FileNotFoundError:
        print("File not found.")
        return None

# 関数定義：データの変更
def change_data(file_path, session_id, name, value):
    try:
        delete_data(file_path, session_id)
        set_data(file_path, session_id, name, value)
        return
    except FileNotFoundError:
        print("File not found.")
        return None

# 関数定義：セッションIDの取得
def session_id_cookie():
    cookie = cookies.SimpleCookie(os.environ.get('COOKIE'))
    cookie = cookie.get('session_id')
    if cookie:
        return cookie.value
    else:
        return None

# 関数定義：ヘッダの設定
def set_header(session_id):
    print("Content-Type: text/html; charset=utf-8")
    print("Content-Language: ja")
    # クッキーの設定
    cookie = cookies.SimpleCookie()
    cookie['session_id'] = session_id  # session_id をクッキーに追加
    cookie['session_id']['expires'] = (datetime.datetime.now() + datetime.timedelta(days=1)).strftime("%a, %d-%b-%Y %H:%M:%S GMT")
    print(str(cookie.output()) + "\r\n\r\n")

# 関数定義：HTMLの設定
def set_html():
    html = '''
<!DOCTYPE html>
<html lang="ja">
<head>
    <meta charset="UTF-8">
    <title>Cookie情報登録</title>
    <link rel=\"stylesheet\" href=\"/root.py/cgi-bin/time.css\">
</head>
<body>
        <header><nav><a href="/">ホーム</a></nav></header>
    
    <section>
        <h3>あなたのsession_id</h3>
        <p>{{ session_id }}</p>
    </section>

    <section>
        <h3>名前とキーワードを登録する</h3>
        <form action="/cgi-bin/session_test.py" method="GET">
            <input type="text" id="name" name="name" placeholder="名前を入力">
            <input type="text" id="value" name="value" placeholder="キーワードを入力">
            <input type="submit" value="登録">
        </form>
        {% if error_str %}
        <p><font color="red">{{ error_str }}</font></p>
        {% endif %}
    </section>
    
    <section>
        <h3>Cookieから情報を確認する</h3>
        <div style="display: flex;">
            <form action="./session_test.py" method="POST">
                <button>sessionの確認</button>
            </form>
            <form action="./session_test.py" method="GET">
                <input type="hidden" id="reset" name="reset" value="reset">
                <button>sessionの削除</button>
            </form>
        </div>

    {% if submitted_name %}
        <h3>Cookieに登録されている情報</h3>
        <p>名前: {{ submitted_name }}</p>
        <p>キーワード: {{ submitted_value }}</p>
    {% endif %}
    </section>

    <section>
        <h3>名前からキーワードを確認する</h3>
        <form action="/cgi-bin/session_test.py" method="GET">
            <input type="text" id="confname" name="confname" placeholder="名前を入力">
            <input type="submit" value="確認">
        </form>
    {% if conf_name %}
        <h3>データベースに登録された情報</h3>
        <p>名前: {{ conf_name }}</p>
        <p>キーワード: {{ conf_value }}</p>
    {% endif %}
    {% if confname_error %}
        <p> <font color="red">{{ confname_error }}</font></p>
    {% endif %}
    </section>

    {% if none_data %}
    <section>
        <p> <font color="red">{{ none_data }}</font></p>
    </section>
    {% endif %}
</body>
</html>
'''
    html += "\r\n\r\n"
    template = Template(html)
    return template

# 関数定義：クエリの確認
def check_query(db_path, session_id):
    query_string = os.environ.get('QUERY_STRING')
    if not query_string:
        return get_data_from_id(db_path, session_id)
    if query_string == 'reset=reset':
        delete_data(db_path, session_id)
        return "KO", ""
    
    query_params = query_string.split('&')
    param_dict = {param.split('=')[0]: param.split('=')[1] for param in query_params if '=' in param}

    if 'name' in param_dict and 'value' in param_dict:
        if param_dict['name'] == '' or param_dict['value'] == '':
            return "Err", ""
        name = urllib.parse.unquote(param_dict['name'])
        value = urllib.parse.unquote(param_dict['value'])
        data = get_data_from_id(db_path, session_id)
        if data[0] == "KO":
            set_data(db_path, session_id, name, value)
        else:
            change_data(db_path, session_id, name, value)
        return "OK", f"{name}, {value}"

    data = get_data_from_id(db_path, session_id)
    return data

# メイン処理
def main():
    # クッキーの取得
    session_id = session_id_cookie()

    # データベースのパス
    db_path = './www/html/root.py/cgi-bin/session_dir/session.db'

    # クエリの確認
    if not session_id:
        session_id = hashlib.sha256(str(time.time()).encode()).hexdigest()

    data = check_query(db_path, session_id)#クエリがあった時
    if data[0] == "OK":
        submitted_name = data[1].split(',')[0]
        submitted_value = data[1].split(',')[1]
    else:
        submitted_name = None
        submitted_value = None
    
    if data[0] == "Err":
        is_error = '名前とキーワードの両方を入力してくれると嬉しいな'
    else:
        is_error = None

    query_string = os.environ.get('QUERY_STRING')#confnameのクエリがあった時
    query_params = query_string.split('&')
    param_dict = {param.split('=')[0]: param.split('=')[1] for param in query_params if '=' in param}
    data = get_data_from_name(db_path, param_dict.get('confname'))
    if data[0] == "OK":
        conf_name = data[1][1].split(',')[0]
        conf_value = data[1][1].split(',')[1]
        confname_error = None
    else:   
        conf_name = None
        conf_value = None
        if data[0] == "Err":
            confname_error = 'その名前に心当たりはないかも'
        else:
            confname_error = None

    if submitted_name is None and conf_name is None:
        none_data = "データがありません"
    else:
        none_data = None

    # HTMLの設定
    set_header(session_id)
    template = set_html()
    data = {
            'session_id' : session_id,
            'submitted_name' : submitted_name,
            'submitted_value' : submitted_value,
            'conf_name' : conf_name,
            'conf_value' : conf_value,
            'confname_error' : confname_error,
            'none_data' : none_data,
            'error_str' : is_error,
            }
    return (template.render(data))


#sys.stderr.write(str(os.environ)) #環境変数の表示
string = main()
#sys.stderr.write(string) #HTMLの表示
print(string)
