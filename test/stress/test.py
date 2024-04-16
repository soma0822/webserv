import requests
import concurrent.futures

url = 'http://localhost:8002/cgi-bin/cookie_test.py'  # 送信先のURLを指定してください
num_requests = 100  # 送信するリクエストの回数

def send_request(url):
    response = requests.get(url)
    return response.text

# 最初のリクエストを送信してレスポンスを取得
reference_response_text = send_request(url)

# 残りのリクエストを並行して送信してレスポンスを確認
with concurrent.futures.ThreadPoolExecutor() as executor:
    futures = [executor.submit(send_request, url) for _ in range(num_requests)]
    for future in concurrent.futures.as_completed(futures):
        response_text = future.result()
        if response_text != reference_response_text:
            print("Responses vary.")
            break
    else:
        print("All responses are the same.")
