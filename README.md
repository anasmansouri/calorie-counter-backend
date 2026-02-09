# Calorie Counter Backend (C++23)

A REST backend for logging foods and meals, built with **C++23** using **Crow** (HTTP server) and **JSON file storage**.

---

## Features

- Foods: CRUD + search by barcode
- Meals: CRUD + filter by name/date/range
- Daily stats endpoint
- Docker / docker-compose 
- Health endpoint for readiness checks

---

## Requirements (local build)

- C++ compiler with C++23 support (e.g. GCC 13+)
- CMake
- libcurl (for HTTP client)
- Asio headers (Crow dependency)
- (Optional) Python + pytest for API integration tests

Ubuntu/Debian:

```bash
sudo apt-get update
sudo apt-get install -y \
  build-essential cmake git pkg-config \
  libcurl4-openssl-dev libasio-dev \
  python3 python3-pip
```

---

## Build (local)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

Binary path:

```text
build/bin/cc_app
```

---

## Run (local)

### DB files

The server uses two JSON files (foods + meals). Set them with env vars:

- `CC_DB_PATH` (foods JSON file path)
- `CC_MEALS_DB_PATH` (meals JSON file path)

Example:

```bash
export CC_DB_PATH=/tmp/cc_foods.json
export CC_MEALS_DB_PATH=/tmp/cc_meals.json

./build/bin/cc_app
```

### Stop the server

The app  will print this:

```text
click Enter to stop it
```

Stop it by pressing **Enter** in the same terminal.

---

## Docker (recommended: docker-compose)

### Build + run

```bash
docker compose up --build
```
### Stop

```bash
docker compose down
```
### DB persistence (volume)

Recommended compose setup (example):

```yaml
services:
  cc_app:
    build: .
    ports:
      - "18080:18080"
    environment:
      CC_DB_PATH: /data/foods.json
      CC_MEALS_DB_PATH: /data/meals.json
    volumes:
      - cc_data:/data
    stdin_open: true
    tty: true

volumes:
  cc_data:
```

---

## API Routes (all endpoints)

Base URL: `http://127.0.0.1:18080`

### General
- `GET /` → `"Hello, Crow!"`
- `GET /health` → `{ "status": "ok" }`

### Foods
- `GET /foods?offset=0&limit=50` → list foods
- `GET /foods/by_barcode?barcode=...` → get a food (local or fetched )
- `POST /foods` → create food
- `PUT /foods` → update food
- `DELETE /foods?barcode=...` → delete one food by barcode
- `DELETE /foods/clear` → delete all foods (from local data base)

**Food JSON (POST/PUT):**
```json
{
  "id": "4250519647774",
  "name": "Oats",
  "brand": "Demo",
  "barcode": "007",
  "caloriePer100g": 389.0,
  "nutrient": [
    {"type":"Protein","unit":"g","value":3.4},
    {"type":"Carbs","unit":"g","value":4.1},
    {"type":"Fat","unit":"g","value":1.5}
  ]
}
```

### Meals
- `GET /meals?offset=0&limit=50` → list meals
- `GET /meals/by_name?name=Lunch` → list meals matching name
- `GET /meals/by_id?id=10` → get meal by id
- `GET /meals/by_date?day=2&month=2&year=2026` → meals on a day
- `GET /meals/by_range?from=YYYY-MM-DD&to=YYYY-MM-DD` → meals in date range
- `POST /meals` → create meal
- `PUT /meals` → update meal
- `DELETE /meals?id=10` → delete meal by id
- `DELETE /meals/clear` → delete all meals

**Meal JSON (POST):**
```json
{
  "name": "Lunch",
  "tsUtc": "2026-02-02T13:05:00Z",
  "foodItems": [["007", 50]]
}
```

**Meal JSON (PUT):**
```json
{
  "id": 1,
  "name": "Dinner",
  "tsUtc": "2026-02-02T20:00:00Z",
  "foodItems": [["007", 40]]
}
```

### Stats
- `GET /stats/day?day=2&month=2&year=2026` → daily summary (e.g. mealsCount, totalCalories, macros)

---

## Quick curl examples

Health:
```bash
curl http://127.0.0.1:18080/health
```

Create a food:
```bash
curl -X POST "http://127.0.0.1:18080/foods" \
  -H "Content-Type: application/json" \
  -d '{"id":"007","name":"Oats","brand":"Demo","barcode":"007","caloriePer100g":389.0,"nutrient":[]}'
```

Create a meal:
```bash
curl -X POST "http://127.0.0.1:18080/meals" \
  -H "Content-Type: application/json" \
  -d '{"name":"Lunch","tsUtc":"2026-02-02T13:05:00Z","foodItems":[["007",50]]}'
```

Daily stats:
```bash
curl "http://127.0.0.1:18080/stats/day?day=2&month=2&year=2026"
```

---

## Tests

### C++ unit tests (Google Test)
to run Ut run only this script :

```bash
./build_and_run_ut.sh
```

or run the following commands :

```bash
cmake -S . -B build-ut -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON
cmake --build build-ut -j
ctest --test-dir build-ut --output-on-failure
```

### API integration tests (pytest)

```bash
python3 -m pip install -U pytest requests
pytest -q tests/test_api
```
