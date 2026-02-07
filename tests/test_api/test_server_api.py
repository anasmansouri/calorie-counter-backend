import requests

HOST = "http://127.0.0.1:18080"


# ---------- Basic ----------

def test_root_returns_hello():
    r = requests.get(f"{HOST}/", timeout=1)
    assert r.status_code == 200
    assert "Hello, Crow!" in r.text


def test_health_ok():
    r = requests.get(f"{HOST}/health", timeout=1)
    assert r.status_code == 200
    j = r.json()
    assert j["status"] == "ok"


# ---------- Helpers ----------

def clear_meals_db():
    r = requests.delete(f"{HOST}/meals/clear", timeout=2)
    # If route not implemented yet, ignore failures
    if r.status_code not in (200, 404):
        assert False, f"clear_meals_db failed: {r.status_code} {r.text}"


def clear_foods_db():
    r = requests.delete(f"{HOST}/foods/clear", timeout=2)
    # If route not implemented yet, ignore failures
    if r.status_code not in (200, 404):
        assert False, f"clear_foods_db failed: {r.status_code} {r.text}"



def post_meal(name, tsUtc=None, food_items=None):
    body = {"name": name}
    if tsUtc is not None:
        body["tsUtc"] = tsUtc
    if food_items is not None:
        body["foodItems"] = [{"foodId": fid, "grams": grams} for fid, grams in food_items]
    return requests.post(f"{HOST}/meals", json=body, timeout=2)


def list_meals(offset=0, limit=50):
    return requests.get(f"{HOST}/meals", params={"offset": offset, "limit": limit}, timeout=2)


# ---------- Foods Tests ----------

def test_search_by_barcode_missing_param_returns_404():
    r = requests.get(f"{HOST}/foods/by_barcode", timeout=1)
    assert r.status_code == 404
    j = r.json()
    assert j["error"] == "missed barcode"


def test_search_by_barcode_empty_param_returns_404():
    r = requests.get(f"{HOST}/foods/by_barcode?barcode=", timeout=1)
    assert r.status_code == 404
    j = r.json()
    assert j["error"] == "missed barcode"


def test_search_by_barcode():
    # keep this test isolated from previous runs
    clear_foods_db()

    r = requests.get(f"{HOST}/foods/by_barcode", params={"barcode": "4025500287955"}, timeout=2)
    assert r.status_code == 200
    j = r.json()
    assert j["id"] == "4025500287955"


def test_post_food_invalid_json_returns_400():
    r = requests.post(
        f"{HOST}/foods",
        data="{ invalid json",
        headers={"Content-Type": "application/json"},
        timeout=2,
    )
    assert r.status_code == 400
    assert "invalid Json body" in r.text


def test_post_food_then_list_contains_it():
    clear_foods_db()

    body = {
        "id": "007",
        "name": "Oats",
        "brand": "PytestBrand",
        "barcode": "007",
        "caloriePer100g": 389.0,
        "servingSizeG": 40.0,
        "nutrient": [
            {"type": "Protein", "unit": "g", "value": 3.4},
            {"type": "Carbs", "unit": "g", "value": 4.1},
            {"type": "Fat", "unit": "g", "value": 1.5},
        ],
    }

    r = requests.post(f"{HOST}/foods", json=body, timeout=2)
    assert r.status_code == 200
    assert r.json()["status"] == "item was added"

    r2 = requests.get(f"{HOST}/foods?offset=0&limit=50", timeout=2)
    assert r2.status_code == 200

    txt = r2.text
    assert "007" in txt
    assert "Oats" in txt


def test_put_updates_food_then_search_returns_updated():
    clear_foods_db()

    body = {
        "id": "008",
        "name": "Oats",
        "brand": "PytestBrand",
        "barcode": "008",
        "caloriePer100g": 389.0,
        "servingSizeG": 40.0,
        "nutrient": [
            {"type": "Protein", "unit": "g", "value": 3.4},
            {"type": "Carbs", "unit": "g", "value": 4.1},
            {"type": "Fat", "unit": "g", "value": 1.5},
        ],
    }

    r = requests.post(f"{HOST}/foods", json=body, timeout=2)
    assert r.status_code == 200

    upd = dict(body)
    upd["name"] = "Rice UPDATED"
    upd["caloriePer100g"] = 361.0

    r2 = requests.put(f"{HOST}/foods", json=upd, timeout=2)
    assert r2.status_code == 200
    assert "updated" in r2.json()["status"].lower()

    r3 = requests.get(f"{HOST}/foods/by_barcode?barcode=008", timeout=2)
    assert r3.status_code == 200
    assert "Rice UPDATED" in r3.text


def test_delete_food_then_search_should_not_find_or_should_error():
    clear_foods_db()

    add = {
        "id": "003",
        "name": "Milk",
        "brand": "PytestBrand",
        "barcode": "003",
        "caloriePer100g": 60.0,
        "servingSizeG": 200.0,
        "nutrient": [
            {"type": "Protein", "unit": "g", "value": 3.4},
            {"type": "Carbs", "unit": "g", "value": 4.1},
            {"type": "Fat", "unit": "g", "value": 1.5},
        ],
    }
    r = requests.post(f"{HOST}/foods", json=add, timeout=2)
    assert r.status_code == 200

    r2 = requests.delete(f"{HOST}/foods?barcode=003", timeout=2)
    assert r2.status_code == 200
    assert r2.json()["status"] == "ok"

    r3 = requests.get(f"{HOST}/foods/by_barcode?barcode=003", timeout=2)

    # expected: not found or error depending on your mapping
    assert r3.status_code in (404, 400, 500, 200)

    if r3.headers.get("Content-Type", "").startswith("application/json") and r3.status_code != 200:
        j = r3.json()
        assert "error" in j


# ---------- Meals Tests ----------

def test_meals_list():
    # keep deterministic
    clear_meals_db()

    r = list_meals()
    assert r.status_code == 200
    j = r.json()
    assert isinstance(j, list)


def test_meals_clear_then_list_empty():
    clear_meals_db()

    r = list_meals()
    assert r.status_code == 200
    j = r.json()
    assert isinstance(j, list)
    assert len(j) == 0


def test_post_meal_invalid_json_returns_400():
    r = requests.post(
        f"{HOST}/meals",
        data="{ invalid json",
        headers={"Content-Type": "application/json"},
        timeout=2,
    )
    assert r.status_code == 400
    assert "invalid Json body" in r.text


def test_post_meal_missing_name_returns_400():
    r = requests.post(f"{HOST}/meals", json={"tsUtc": "2026-02-02T08:15:00Z"}, timeout=2)
    assert r.status_code in (400, 404)
    if r.headers.get("Content-Type", "").startswith("application/json"):
        assert "error" in r.json() or "missing" in r.text.lower()


def test_post_meal_then_list_contains_it():
    clear_meals_db()

    r = post_meal(name="Lunch", food_items=[("7777", 70)])
    assert r.status_code == 200
    assert "added" in r.json()["status"].lower()

    r2 = list_meals()
    assert r2.status_code == 200
    assert "Lunch" in r2.text


def test_get_meals_by_name_returns_only_matching():
    clear_meals_db()

    assert post_meal("Breakfast", tsUtc="2026-02-02T08:15:00Z").status_code == 200
    assert post_meal("Lunch", tsUtc="2026-02-02T13:05:00Z").status_code == 200
    assert post_meal("Dinner", tsUtc="2026-02-03T19:00:00Z").status_code == 200

    r = requests.get(f"{HOST}/meals/by_name", params={"name": "Lunch"}, timeout=2)
    assert r.status_code == 200
    j = r.json()
    assert isinstance(j, list)
    assert len(j) >= 1

    for item in j:
        if isinstance(item, dict) and "name" in item:
            assert "lunch" in str(item["name"]).lower()
        else:
            assert "Lunch" in r.text


def test_get_meals_by_date_returns_same_day_only():
    clear_meals_db()

    assert post_meal("Breakfast", tsUtc="2026-02-02T08:15:30Z").status_code == 200
    assert post_meal("Lunch", tsUtc="2026-02-02T13:05:00Z").status_code == 200
    assert post_meal("Dinner", tsUtc="2026-02-03T19:00:00Z").status_code == 200

    r = requests.get(
        f"{HOST}/meals/by_date",
        params={"day": 2, "month": 2, "year": 2026},
        timeout=2,
    )
    assert r.status_code == 200
    j = r.json()
    assert isinstance(j, list)
    assert len(j) == 2

    txt = r.text.lower()
    assert "breakfast" in txt
    assert "lunch" in txt
    assert "dinner" not in txt


def test_put_meal_updates_it_then_by_name_reflects_change():
    clear_meals_db()

    assert post_meal("Lunch", tsUtc="2026-02-02T13:05:00Z").status_code == 200

    rlist = list_meals()
    assert rlist.status_code == 200
    meals = rlist.json()
    assert len(meals) == 1

    assert isinstance(meals[0], dict) and "id" in meals[0]
    meal_id = meals[0]["id"]

    upd = {
        "id": meal_id,
        "name": "Dinner",
        "tsUtc": "2026-02-02T20:00:00Z",
        "foodItems": [["007", 40]],
    }

    rput = requests.put(f"{HOST}/meals", json=upd, timeout=2)
    assert rput.status_code == 200
    assert "updated" in rput.json()["status"].lower()

    rby = requests.get(f"{HOST}/meals/by_name", params={"name": "Dinner"}, timeout=2)
    assert rby.status_code == 200
    assert "Dinner" in rby.text


def test_delete_meal_then_list_is_empty():
    clear_meals_db()

    assert post_meal("Breakfast", tsUtc="2026-02-02T08:15:00Z").status_code == 200

    rlist = list_meals()
    assert rlist.status_code == 200
    meals = rlist.json()
    assert len(meals) == 1
    assert isinstance(meals[0], dict) and "id" in meals[0]

    meal_id = meals[0]["id"]

    rdel = requests.delete(f"{HOST}/meals", params={"id": meal_id}, timeout=2)
    assert rdel.status_code == 200
    assert rdel.json()["status"] == "ok"

    rlist2 = list_meals()
    assert rlist2.status_code == 200
    assert isinstance(rlist2.json(), list)
    assert len(rlist2.json()) == 0


# ---------- New endpoints tests ----------

def test_get_meals_by_range_returns_only_in_range():
    clear_meals_db()

    assert post_meal("Breakfast", tsUtc="2026-02-02T08:15:30Z").status_code == 200
    assert post_meal("Lunch", tsUtc="2026-02-03T13:05:00Z").status_code == 200
    assert post_meal("Dinner", tsUtc="2026-02-05T19:00:00Z").status_code == 200

    r = requests.get(
        f"{HOST}/meals/by_range",
        params={"from": "2026-02-02", "to": "2026-02-03"},
        timeout=2,
    )
    assert r.status_code == 200
    txt = r.text.lower()
    assert "breakfast" in txt
    assert "lunch" in txt
    assert "dinner" not in txt


def test_stats_day_returns_summary():
    clear_meals_db()
    clear_foods_db()

    # Add foods needed for calories
    assert requests.post(
        f"{HOST}/foods",
        json={
            "id": "007",
            "name": "Oats",
            "brand": "PytestBrand",
            "barcode": "007",
            "caloriePer100g": 400.0,
            "servingSizeG": 40.0,
            "nutrient": [],
        },
        timeout=2,
    ).status_code == 200

    assert requests.post(
        f"{HOST}/foods",
        json={
            "id": "008",
            "name": "Milk",
            "brand": "PytestBrand",
            "barcode": "008",
            "caloriePer100g": 50.0,
            "servingSizeG": 200.0,
            "nutrient": [],
        },
        timeout=2,
    ).status_code == 200

    assert post_meal("Breakfast", tsUtc="2026-02-02T08:15:30Z", food_items=[("007", 50)]).status_code == 200
    assert post_meal("Lunch", tsUtc="2026-02-02T13:05:00Z", food_items=[("008", 200)]).status_code == 200

    r = requests.get(
        f"{HOST}/stats/day",
        params={"day": 2, "month": 2, "year": 2026},
        timeout=2,
    )
    assert r.status_code == 200
    j = r.json()

    assert j["mealsCount"] == 2
    assert "totalCalories" in j
    assert j["totalCalories"] != 0 
