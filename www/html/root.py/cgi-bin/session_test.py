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

<<<<<<< HEAD
def write_string_by_id(file_path, id_, name, value):
=======
def write_string_by_id(file_path, id_, string):
>>>>>>> a9bc9ef (feat: session一旦できた。)
    try:
        with open(file_path, 'a') as file:
            fd = file.fileno()
            fcntl.flock(fd, fcntl.LOCK_EX)  # 排他ロック
<<<<<<< HEAD
            file.write(f"{id_},{name},{value}\n")
=======
            file.write(f"{id_},{string},\n")
>>>>>>> a9bc9ef (feat: session一旦できた。)
            fcntl.flock(fd, fcntl.LOCK_UN)  # ロック解除
    except Exception as e:
        sys.stderr.write(f"An error occurred: {e}")

def get_data(file_path, target_id):
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

<<<<<<< HEAD
def get_data_from_name(file_path, name):
    if not name:
        return "KO", ""
    name = urllib.parse.unquote(name)
=======
#def get_data_from_name(file_path, name):
#    try:
#        with open(file_path, 'r') as file:
#            for line in file:
#                parts = line.strip().split(',',1)
#                if name == parts[1]:
#                    return "OK",parts
#            return "KO", ""
#    except FileNotFoundError:
#        print("File not found.")
#        return None
#

def get_data_from_name(file_path, name):
>>>>>>> a9bc9ef (feat: session一旦できた。)
    try:
        with open(file_path, 'r') as file:
            for line in file:
                parts = line.strip().split(',', 1)
                if len(parts) >= 2 and name == parts[1].split(',')[0]:
                    return "OK", parts
            return "KO", ""
    except FileNotFoundError:
        print("File not found.")
        return None

def delete_cookie(file_path, session_id):
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()
        with open(file_path, 'w') as file:
            for line in lines:
                if session_id not in line:
                    file.write(line)
    except FileNotFoundError:
        print("File not found.")
        return None

def insert_data(file_path, session_id, name, value):
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()
        with open(file_path, 'w') as file:
            fd = file.fileno()
            for line in lines:
                if session_id == line.split(',')[0]:
                    fcntl.flock(fd, fcntl.LOCK_EX)  # 排他ロック
                    file.write(f"{session_id},{name},{value}\n")
                    fcntl.flock(fd, fcntl.LOCK_UN)  # ロック解除
                    return
                else:
                    file.write(line)
        return
    except FileNotFoundError:
        print("File not found.")
        return None

# 関数定義：クッキーの取得
def visit_count_cookie():
    cookie = cookies.SimpleCookie(os.environ.get('COOKIE'))
    visit_count = cookie.get('visit_count')
    # resetのクエリがある場合、訪問回数を更新
    if os.environ.get('QUERY_STRING') == 'wreset':
        visit_count = 0
    if visit_count:
        return visit_count.value
    else:
        return 0

def session_id_cookie():
    cookie = cookies.SimpleCookie(os.environ.get('COOKIE'))
    cookie = cookie.get('session_id')
    if cookie:
        return cookie.value
    else:
        return None

def set_header(n, session_id):
    print("Content-Type: text/html; charset=utf-8")
    print("Content-Language: ja")
    # クッキーの設定
    cookie = cookies.SimpleCookie()
    cookie['visit_count'] = str(n)
    cookie['session_id'] = session_id  # session_id をクッキーに追加
    cookie['session_id']['expires'] = (datetime.datetime.now() + datetime.timedelta(days=1)).strftime("%a, %d-%b-%Y %H:%M:%S GMT")
    print(str(cookie.output()) + "\r\n\r\n")


def set_html():
    html = '''
<!DOCTYPE html>
<html lang="ja">
<head>
    <meta charset="UTF-8">
<<<<<<< HEAD
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
            <label for="name">名前:</label>
            <input type="text" id="name" name="name" placeholder="名前を入力">
            <label for="value">キーワード:</label>
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
            <form id="myForm" action="/cgi-bin/session_test.py" method="GET">
                <button>sessionの確認</button>
            </form>
            <form action="/cgi-bin/session_test.py" method="GET">
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
        <h3>名前を基にキーワードを確認する</h3>
        <form action="/cgi-bin/session_test.py" method="GET">
            <label for="confname">名前からキーワードを確認:</label>
            <input type="text" id="confname" name="confname" placeholder="名前を入力">
            <input type="submit" value="確認">
        </form>
    {% if conf_name %}
        <h3>データベースに登録された情報</h3>
        <p>名前: {{ conf_name }}</p>
        <p>キーワード: {{ conf_value }}</p>
    {% endif %}
    </section>

    {% if none_data %}
    <section>
        <p> <font color="red">{{ none_data }}</font></p>
    </section>
=======
    <title>My Cute Webpage</title>
</head>
<body>
    <h3>session_id = {{ session_id }}</h3>
    <h2>My Cute Webpage</h2>

    <!-- 名前を入力するフォーム -->
    <h3>名前からデータベースから確認</h3>
    <form action="/cgi-bin/session_test.py" method="GET">
        <label for="name">名前からキーワードを確認する:</label>
        <input type="text" id="confname" name="confname">
        <input type="submit" value="確認">
    </form>

    <!-- 名前とキーワードを入力するフォーム -->
    <h3>名前とキーワードをcokkieに紐付けて、データベースに記録する</h3>
    <form action="/cgi-bin/session_test.py" method="GET">
        <label for="name"名前を入力してね:</label>
        <input type="text" id="name" name="name">
        <label for="value">キーワードを入力してね:</label>
        <input type="text" id="value" name="value">
        <input type="submit" value="登録する">
    </form>

    <!-- フォーム -->
    <h3>cokkieから確認する</h3>
    <form id="myForm" action="/cgi-bin/session_test.py" method="GET">
        <!-- ボタンをフォームの内部に配置 -->
        <button>再確認</button>
    </form>

    <!-- 入力された名前を表示する欄 -->
    {% if submitted_name %}
    <p>名前: {{ submitted_name }}</p>
    <p>キーワード: {{ submitted_value }}</p>
    {% endif %}
    {% if conf_name %}
    <p>名前: {{ conf_name }}</p>
    <p>キーワード: {{ conf_value }}</p>
>>>>>>> a9bc9ef (feat: session一旦できた。)
    {% endif %}
</body>
</html>
'''
    html += "\r\n\r\n"
    template = Template(html)
    return template

def check_query(db_path, session_id):
    query_string = os.environ.get('QUERY_STRING')
<<<<<<< HEAD
    if not query_string:
        return get_data(db_path, session_id)
    if query_string == 'reset=reset':
=======
    sys.stderr.write(query_string)
    if not query_string:
        return get_data(db_path, session_id)
    if query_string == 'reset':
>>>>>>> a9bc9ef (feat: session一旦できた。)
        delete_cookie(db_path, session_id)
        return "KO", ""
    
    query_params = query_string.split('&')
    param_dict = {param.split('=')[0]: param.split('=')[1] for param in query_params if '=' in param}

    if 'name' in param_dict and 'value' in param_dict:
        if param_dict['name'] == '' or param_dict['value'] == '':
<<<<<<< HEAD
            return "Err", ""
        name = urllib.parse.unquote(param_dict['name'])
        value = urllib.parse.unquote(param_dict['value'])
        data = get_data(db_path, session_id)
        if data[0] == "KO":
            write_string_by_id(db_path, session_id, name, value)
        else:
            insert_data(db_path, session_id, name, value)
        return "OK", f"{name}, {value}"

    data = get_data(db_path, session_id)
    return data
=======
            return "KO", ""
        name = urllib.parse.unquote(param_dict['name'])
        value = urllib.parse.unquote(param_dict['value'])
        insert_data(db_path, session_id, name, value)
        return "OK", f"{name}, {value}"
    else:
        return "KO", ""
#def check_query(db_path, session_id):
#    query_string = os.environ.get('QUERY_STRING')
#    sys.stderr.write(query_string)
#    if not query_string:
#        sys.stderr.write("2")
#        return "KO", ""
#    if query_string == 'reset':
#        delete_cookie(db_path, session_id)
#        sys.stderr.write("3")
#        return "KO", ""
#    if query_string.find('confname') == 1:
#        sys.stderr.write(query_string.split('=')[1])
#        data = get_data_from_name(db_path, query_string.split('=')[1])
#        sys.stderr.write("4")
#        return data
#    if query_string.find('name') == 1 and query_string.find('value') == 1:
#        name = query_string.split('=')[1]
#        value = query_string.split('=')[2]
#        insert_data(db_path, session_id, name, value)
#        sys.stderr.write("5")
#        return "OK", "{name}, {value}"
#    else:
#        sys.stderr.write("6")
#        return "KO", ""
            
    

>>>>>>> a9bc9ef (feat: session一旦できた。)

# メイン処理
def main():
    # クッキーの取得
    session_id = session_id_cookie()
    visit_count = int(visit_count_cookie()) + 1

    # データの取得
    db_path = '/Users/kaaaaakun_/Desktop/42tokyo/webserv/www/html/root.py/cgi-bin/session_dir/session.db'

<<<<<<< HEAD
    if not session_id:
        session_id = hashlib.sha256(str(time.time()).encode()).hexdigest()

    data = check_query(db_path, session_id)#クエリがあった時
=======
    if session_id:
        data = get_data(db_path, session_id)
        if data[0] == "KO":
            write_string_by_id(db_path, session_id, '')
    else:
        session_id = hashlib.sha256(str(time.time()).encode()).hexdigest()
        write_string_by_id(db_path, session_id, '')

    data = check_query(db_path, session_id)
>>>>>>> a9bc9ef (feat: session一旦できた。)
    if data[0] == "OK":
        submitted_name = data[1].split(',')[0]
        submitted_value = data[1].split(',')[1]
    else:
        submitted_name = None
        submitted_value = None
<<<<<<< HEAD
    
    if data[0] == "Err":
        is_error = '値は両方入力してください'
    else:
        is_error = None
        

    query_string = os.environ.get('QUERY_STRING')#confnameのクエリがあった時
=======

    query_string = os.environ.get('QUERY_STRING')
>>>>>>> a9bc9ef (feat: session一旦できた。)
    query_params = query_string.split('&')
    param_dict = {param.split('=')[0]: param.split('=')[1] for param in query_params if '=' in param}
    data = get_data_from_name(db_path, param_dict.get('confname'))
    if data[0] == "OK":
        conf_name = data[1][1].split(',')[0]
        conf_value = data[1][1].split(',')[1]
    else:   
        conf_name = None
        conf_value = None

<<<<<<< HEAD
    if submitted_name is None and conf_name is None:
        none_data = "データがありません"
        sys.stderr.write(none_data)
    else:
        none_data = None
        sys.stderr.write("データがあります")

=======
>>>>>>> a9bc9ef (feat: session一旦できた。)
    # HTMLの設定
    set_header(visit_count, session_id)
    template = set_html()
    data = {
            'session_id' : session_id,
            'submitted_name' : submitted_name,
            'submitted_value' : submitted_value,
            'conf_name' : conf_name,
            'conf_value' : conf_value,
<<<<<<< HEAD
            'none_data' : none_data,
            'error_str' : is_error,
=======
>>>>>>> a9bc9ef (feat: session一旦できた。)
            }
    return (template.render(data))


# insert_data('4','Alice', 'Pizza', 'Summer')
#sys.stderr.write(str(os.environ)) #環境変数の表示
string = main()
#データを挿入し、割り当てられたIDを取得
#sys.stderr.write(string) #HTMLの表示
print(string)
