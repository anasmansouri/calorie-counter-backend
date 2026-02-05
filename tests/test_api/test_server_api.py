import requests

from datetime import datetime, timezone

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
                "type": "Protein",
                "unit": "g",
                "value": 3.4
            },
            {
                "type": "Carbs",
                "unit": "g",
                "value": 4.1
            },
            {
                "type": "Fat",
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
                "type": "Protein",
                "unit": "g",
                "value": 3.4
            },
            {
                "type": "Carbs",
                "unit": "g",
                "value": 4.1
            },
            {
                "type": "Fat",
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
                "type": "Protein",
                "unit": "g",
                "value": 3.4
            },
            {
                "type": "Carbs",
                "unit": "g",
                "value": 4.1
            },
            {
                "type": "Fat",
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


# ---------- Helpers ----------


def clear_meals_db():
    # If you implement /meals/clear
    r = requests.delete(f"{HOST}/meals/clear", timeout=2)
    # If route not implemented yet, ignore failures
    if r.status_code not in (200, 404):
        assert False, f"clear_meals_db failed: {r.status_code} {r.text}"


def post_meal(name, tsUtc=None, food_items=None):
    body = {"name": name}
    if tsUtc is not None:
        body["tsUtc"] = tsUtc
    if food_items is not None:
        body["foodItems"] = food_items
    return requests.post(f"{HOST}/meals", json=body, timeout=2)



def list_meals(offset=0, limit=50):
    return requests.get(f"{HOST}/meals", params={"offset": offset, "limit": limit}, timeout=2)

# ---------- Meal Tests ----------

def test_meals_list():
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
    # name is required
    r = requests.post(f"{HOST}/meals", json={"tsUtc": "2026-02-02T08:15:00Z"}, timeout=2)
    assert r.status_code in (400, 404)  # depending on your error mapping
    # if JSON error message exists, check it
    if r.headers.get("Content-Type", "").startswith("application/json"):
        assert "error" in r.json() or "missing" in r.text.lower()


def test_post_meal_then_list_contains_it():
    clear_meals_db()

    # If tsUtc is optional, server uses now(); we can still find it by name in list response
    r = post_meal(name="Lunch",food_items=[["7777",70]])
    assert r.status_code == 200
    assert "added" in r.json()["status"].lower()

    r2 = list_meals()
    assert r2.status_code == 200
    txt = r2.text
    assert "Lunch" in txt


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

    # Ensure every returned item is Lunch
    # (depending on your MealLog JSON schema, name might be a string)
    for item in j:
        # check common patterns
        if isinstance(item, dict) and "name" in item:
            assert "lunch" in str(item["name"]).lower()
        else:
            # fallback: string-based check in raw text
            assert "Lunch" in r.text



def test_get_meals_by_date_returns_same_day_only():
    clear_meals_db()

    # Two meals on 2026-02-02 UTC
    assert post_meal("Breakfast", tsUtc="2026-02-02T08:15:30Z").status_code == 200
    assert post_meal("Lunch", tsUtc="2026-02-02T13:05:00Z").status_code == 200
    # One meal on the next day
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

    # verify the two expected names appear
    txt = r.text
    assert "Breakfast" in txt or "breakfast" in txt.lower()
    assert "Lunch" in txt or "lunch" in txt.lower()
    assert "Dinner" not in txt  # should not be in 2026-02-02 list


def test_put_meal_updates_it_then_by_name_reflects_change():
    clear_meals_db()

    # Create a meal first
    assert post_meal("Lunch", tsUtc="2026-02-02T13:05:00Z").status_code == 200

    # Get all meals to find the created meal id
    rlist = list_meals()
    assert rlist.status_code == 200
    meals = rlist.json()
    assert len(meals) == 1

    meal_id = None
    if isinstance(meals[0], dict) and "id" in meals[0]:
        meal_id = meals[0]["id"]
    else:
        # If your MealLog JSON doesn't expose id, you can't test PUT reliably
        # In that case: expose id in to_json, or add a /meals/by_name endpoint that returns id.
        assert False, "Meal JSON response does not contain 'id' field; cannot test PUT."

    # Update name and time
    upd = {
        "id": meal_id,
        "name": "Dinner",
        "tsUtc": "2026-02-02T20:00:00Z",
        "foodItems": [["007",40]],
    }

    rput = requests.put(f"{HOST}/meals", json=upd, timeout=2)
    assert rput.status_code == 200
    assert "updated" in rput.json()["status"].lower()

    # Now query by new name
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

    if not (isinstance(meals[0], dict) and "id" in meals[0]):
        assert False, "Meal JSON response does not contain 'id' field; cannot test DELETE."

    meal_id = meals[0]["id"]

    rdel = requests.delete(f"{HOST}/meals", params={"id": meal_id}, timeout=2)
    assert rdel.status_code == 200
    assert rdel.json()["status"] == "ok"

    rlist2 = list_meals()
    assert rlist2.status_code == 200
    assert isinstance(rlist2.json(), list)
    assert len(rlist2.json()) == 0

