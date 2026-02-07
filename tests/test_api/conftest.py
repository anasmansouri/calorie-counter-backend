import os
import time
import signal
import subprocess
import requests
import pytest

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "../.."))

SERVER_BIN = os.path.join(PROJECT_ROOT, "build", "bin", "cc_app")

HOST = "http://127.0.0.1:18080"

env = os.environ.copy()
env["CC_DB_PATH"] = "/tmp/cc_test_db.json"
env["CC_MEALS_DB_PATH"] = "/tmp/cc_UT_test_service_meal_db.json"
def _wait_until_ready(timeout_s: float = 8.0) -> None:
    t0 = time.time()
    last_err = None
    while time.time() - t0 < timeout_s:
        try:
            r = requests.get(f"{HOST}/health", timeout=0.5)
            if r.status_code == 200:
                return
        except Exception as e:
            last_err = e
        time.sleep(0.1)
    raise RuntimeError(f"Server did not become ready. Last error: {last_err}")


@pytest.fixture(scope="session", autouse=True)
def server_process():
    if not os.path.exists(SERVER_BIN):
        raise RuntimeError(
            f"Server binary not found at: {SERVER_BIN}\n"
            "Build it first, or update SERVER_BIN in tests/test_api/conftest.py."
        )

    log_path = os.path.join(os.path.dirname(__file__), "server.log")
    err_path = os.path.join(os.path.dirname(__file__), "server.err")

    with open(log_path, "w") as out, open(err_path, "w") as err:
        p = subprocess.Popen([SERVER_BIN], stdout=out, stderr=err, env=env,stdin=subprocess.PIPE)
            
        try:
            _wait_until_ready()
            yield p
        finally:
            try:
                p.send_signal(signal.SIGINT)
                p.wait(timeout=3)
            except Exception:
                p.kill()

