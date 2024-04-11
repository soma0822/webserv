import requests

BASE_URL = "http://localhost:8002"


def test_client_session_state():
    s = requests.Session()
    r1 = s.get(f"{BASE_URL}/cgi-bin/cookie_test.py")
    r2 = s.get(f"{BASE_URL}/cgi-bin/cookie_test.py")

    assert int(r1.cookies["visit_count"]) == int(r2.cookies["visit_count"]) - 1


def test_database_session():
    params = {"name": "test_name", "value": "test_value"}
    s = requests.Session()
    s.get(f"{BASE_URL}/cgi-bin/session_test.py", params=params)

    r = s.post(f"{BASE_URL}/cgi-bin/session_test.py")
    assert "test_name" in r.text
    assert "test_value" in r.text
