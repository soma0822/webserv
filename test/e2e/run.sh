#!/bin/bash

# Usageにエラーがあれば即座に終了
# テストに失敗があっても終了しない

# 出力の色を定義
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

success=0
failure=0
test_server_pid=0

function test() {
  host_addr=$1 # host:port

  test_requests "$host_addr" test/e2e/request/bad_request_400
  test_requests "$host_addr" test/e2e/request/ok_200
}

function test_requests() {
  host_addr=$1 # host:port
  test_dir=$2
  expected_status=$(echo "$test_dir" | awk -F'_' '{print $NF}')

  # 引数のチェック
  if [ -z "$host_addr" ] || [ -z "$test_dir" ] ; then
    echo -e "${RED}エラー: host_addr, test_dir, expected_statusは必須です${NC}" >&2
    exit 1
  fi

  # テストディレクトリの存在チェック
  if [ ! -d "$test_dir" ]; then
    echo -e "${RED}エラー: test_dirが存在しません${NC}" >&2
    exit 1
  fi

  # テストディレクトリ内のファイルを取得
  test_files=$(find "$test_dir" -type f)

  # 全てのファイルにつき、requestを実行
  for test_file in $test_files; do
    response=$(request "$host_addr" "$test_file")

    status=$(echo "$response" | head -n 1 | awk '{print $2}')
    if [ "$status" != "$expected_status" ]; then
      echo -e "${RED}failed: $test_file${NC}" >&2
      echo -e "${RED}Expected: $expected_status${NC}" >&2
      echo -e "${RED}Actual: $status${NC}" >&2
      failure=$((failure + 1))
    else
      echo -e "${GREEN}passed: $test_file${NC}"
      success=$((success + 1))
    fi
  done
}

function request() {
  host_addr=$1
  request_file=$2

  # 引数のチェック
  if [ -z "$host_addr" ] || [ -z "$request_file" ]; then
    echo -e "${RED}エラー: host, port and request_fileは必須です${NC}" >&2
    exit 1
  fi

  cat "$request_file" | awk 1 ORS='\r\n' | curl -m 1 telnet://"$host_addr" 2> /dev/null || true
}

function launch_test_server() {
  # テスト用のサーバーを起動
  config_file=$1

  # 引数のチェック
  if [ -z "$config_file" ]; then
    echo -e "${RED}エラー: host_addr and config_fileは必須です${NC}" >&2
    exit 1
  fi

  # サーバーの起動
  ./webserv_debug "$config_file" &

  # サーバーの起動待ち
  sleep 2
}

function cleanup() {
  if [ "$test_server_pid" -ne 0 ]; then
    kill -9 "$test_server_pid"
  fi
}

function main() {
  config_file=$1
  host_addr=$2

  # 引数のチェック
  if [ -z "$config_file" ] || [ -z "$host_addr" ]; then
    echo -e "${RED}エラー: config_file and host_addrは必須です${NC}" >&2
    exit 1
  fi

  # テストサーバーの起動
  launch_test_server "$config_file"

  # テストの実行
  echo "Testing..." 1>&2
  test "$host_addr"

  # テスト結果の出力
  echo -e "${GREEN}Success: $success${NC}"
  echo -e "${RED}Failure: $failure${NC}"

  if [ "$failure" -ne 0 ]; then
    exit 1
  fi

  cleanup
}

main conf/default.conf "localhost:8002"
