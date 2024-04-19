import pytest
import subprocess
import time


@pytest.fixture(scope='session', autouse=True)
def setup_before_tests(request):
    server = subprocess.Popen(['./webserv_debug'])
    time.sleep(1)
    yield
    server.kill()
