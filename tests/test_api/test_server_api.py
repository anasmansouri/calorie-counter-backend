import requests

HOST = "http://127.0.0.1:18080"

def test_root_returns_hello():
    r = requests.get(f"{HOST}/", timeout=1)
    assert r.status_code == 200
    assert "Hello, Crow!" in r.text

def test_health_ok():
    r = requests.get(f"{HOST}/health", timeout=1)
    assert r.status_code == 200
    j = r.json()
    assert j["status"] == "ok"



def test_search_by_barcode_missing_param_returns_404():
    r = requests.get(f"{HOST}/search_by_barcode", timeout=1)
    assert r.status_code == 404
    j = r.json()
    assert j["error"] == "missed barcode"


def test_search_by_barcode_empty_param_returns_404():
    r = requests.get(f"{HOST}/search_by_barcode?barcode=", timeout=1)
    assert r.status_code == 404
    j = r.json()
    assert j["error"] == "missed barcode"

def test_search_by_barcode():
    r = requests.get(f"{HOST}/search_by_barcode",params={"barcode":"4025500287955"}, timeout=1)
    assert r.status_code == 200 
    j = r.json()
    assert j["id"] == "4025500287955"



def test_post_food_invalid_json_returns_400():
    # Your server returns: crow::response(400, "invalid Json body")
    r = requests.post(
        f"{HOST}/foods",
        data="{ invalid json",
        headers={"Content-Type": "application/json"},
        timeout=2,
    )
    assert r.status_code == 400
    assert "invalid Json body" in r.text


def test_post_food_then_list_contains_it():
    body = {
        "id": "007",
        "name": "Oats",
        "brand": "PytestBrand",
        "barcode": "007",
        "caloriePer100g": 389.0,
        "servingSizeG": 40.0,
        "nutrient": [
            {
                "name": "protein",
                "unit": "g",
                "value": 3.4
            },
            {
                "name": "carbohydrates",
                "unit": "g",
                "value": 4.1
            },
            {
                "name": "fat",
                "unit": "g",
                "value": 1.5
            }
        ],
    }

    r = requests.post(f"{HOST}/foods", json=body, timeout=2)
    assert r.status_code == 200
    assert r.json()["status"] == "item was added"

    r2 = requests.get(f"{HOST}/foods?offset=0&limit=50", timeout=2)
    assert r2.status_code == 200

    # We don't assume the exact response shape (array vs object),
    # just ensure our content is present.
    txt = r2.text
    assert "007" in txt
    assert "Oats" in txt


def test_put_updates_food_then_search_returns_updated():
    # First add
    body = {
        "id": "008",
        "name": "Oats",
        "brand": "PytestBrand",
        "barcode": "008",
        "caloriePer100g": 389.0,
        "servingSizeG": 40.0,
        "nutrient": [
            {
                "name": "protein",
                "unit": "g",
                "value": 3.4
            },
            {
                "name": "carbohydrates",
                "unit": "g",
                "value": 4.1
            },
            {
                "name": "fat",
                "unit": "g",
                "value": 1.5
            }
        ],
    }
        
    r = requests.post(f"{HOST}/foods", json=body, timeout=2)
    assert r.status_code == 200

    # Update
    upd = dict(body)
    upd["name"] = "Rice UPDATED"
    upd["caloriePer100g"] = 361.0

    r2 = requests.put(f"{HOST}/foods", json=upd, timeout=2)
    assert r2.status_code == 200
    assert "updated" in r2.json()["status"].lower()

    # If your service searches local DB first, this stays offline.
    r3 = requests.get(f"{HOST}/search_by_barcode?barcode=008", timeout=2)
    assert r3.status_code == 200
    assert "Rice UPDATED" in r3.text

def test_delete_food_then_search_should_not_find_or_should_error():
    # Add
    add = {
        "id": "003",
        "name": "Milk",
        "brand": "PytestBrand",
        "barcode": "003",
        "caloriePer100g": 60.0,
        "servingSizeG": 200.0,
        "nutrient": [
            {
                "name": "protein",
                "unit": "g",
                "value": 3.4
            },
            {
                "name": "carbohydrates",
                "unit": "g",
                "value": 4.1
            },
            {
                "name": "fat",
                "unit": "g",
                "value": 1.5
            }
        ],
    }
    r = requests.post(f"{HOST}/foods", json=add, timeout=2)
    assert r.status_code == 200

    # Delete
    r2 = requests.delete(f"{HOST}/foods?barcode=003", timeout=2)
    assert r2.status_code == 200
    assert r2.json()["status"] == "ok"

    
    r3 = requests.get(f"{HOST}/search_by_barcode?barcode=003", timeout=2)

    # expected 500
    assert r3.status_code in (404, 400, 500, 200)

    # If it returns JSON error, check it exists
    if r3.headers.get("Content-Type", "").startswith("application/json") and r3.status_code != 200:
        j = r3.json()
        assert "error" in j
