#!/bin/bash

# Usageにエラーがあれば即座に終了
# テストに失敗があっても終了しない

# 出力の色を定義
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

success=0
failure=0

function test() {
  host_addr=$1 # host:port
  test_dir=$2
  expected_status=$3

  # 引数のチェック
  if [ -z "$host_addr" ] || [ -z "$test_dir" ] || [ -z "$expected_status" ]; then
    echo -e "${RED}Error: host_addr, test_dir and expected_status are required${NC}" >&2
    exit 1
  fi

  # テストディレクトリの存在チェック
  if [ ! -d "$test_dir" ]; then
    echo -e "${RED}Error: test_dir does not exist${NC}" >&2
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
    echo -e "${RED}Error: host, port and request_file are required${NC}" >&2
    exit 1
  fi

  cat "$request_file" | awk 1 ORS='\r\n' | curl -m 1 telnet://"$host_addr" 2> /dev/null || true
}

function main() {

  # テストの実行
  test localhost:8002 test/e2e/request/bad_request "400"

  # テスト結果の出力
  echo -e "${GREEN}Success: $success${NC}"
  echo -e "${RED}Failure: $failure${NC}"

  if [ "$failure" -ne 0 ]; then
    exit 1
  fi
}

main "$@"
