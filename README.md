# Calorie Counter Backend (C++23)

A small REST backend for logging foods and meals, built with **C++23** using **Crow** (HTTP server) and **JSON file storage**.

## Features

- Foods:
  - List foods
  - Find food by barcode (local DB first, optionally fetch from OpenFoodFacts client)
  - Create / update / delete foods
- Meals:
  - List meals
  - Find meals by name / id / date
  - Create / update / delete meals
  - Clear meals DB (dev/testing)
- Health endpoint for readiness checks

---

## Requirements (local build)

- C++ compiler with C++23 support (e.g. GCC 13+)
- CMake
- libcurl (for HTTP client)
- Asio headers (Crow dependency)
- (Optional) Python + pytest for API integration tests

On Ubuntu/Debian you typically need:

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake git pkg-config libcurl4-openssl-dev libasio-dev python3 python3-pip
```

---

## Build

From the repository root:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

The server binary is expected at:

```text
build/bin/cc_app
```

---

## Run

### Database files

The server uses two JSON files:
- foods DB
- meals DB

You can set them with environment variables:

- `CC_FOODS_DB_PATH` (foods JSON file path)
- `CC_MEALS_DB_PATH` (meals JSON file path)

Example:

```bash
export CC_FOODS_DB_PATH=/tmp/cc_foods.json
export CC_MEALS_DB_PATH=/tmp/cc_meals.json

./build/bin/cc_app
```

> Tip: The app creates the files if they do not exist.

### Stopping the server

Right now the server waits for **Enter** in the terminal:

```text
serving running , click Enter to stop it :
```

So you stop it by pressing Enter.

---

## Docker

### Build and run (interactive)
Because the app waits for Enter, run Docker **interactively**:

```bash
docker build -t cc_app .
docker run --rm -it -p 18080:18080 \
  -e CC_FOODS_DB_PATH=/data/foods.json \
  -e CC_MEALS_DB_PATH=/data/meals.json \
  -v cc_data:/data \
  cc_app
```

Then test:

```bash
curl http://127.0.0.1:18080/health
```

> If you don’t want interactive mode, change `main.cpp` to not block on stdin (e.g. use `isatty()` or wait for SIGINT/SIGTERM).

---

## API

Base URL: `http://127.0.0.1:18080`

### Health
```bash
curl http://127.0.0.1:18080/health
```

### Foods

#### List foods
`GET /foods?offset=0&limit=50`

```bash
curl "http://127.0.0.1:18080/foods?offset=0&limit=50"
```

#### Find by barcode
`GET /foods/by_barcode?barcode=4025500287955`

```bash
curl "http://127.0.0.1:18080/foods/by_barcode?barcode=4025500287955"
```

#### Create food
`POST /foods`

```bash
curl -X POST "http://127.0.0.1:18080/foods" \
  -H "Content-Type: application/json" \
  -d '{
    "id": "007",
    "name": "Oats",
    "brand": "Demo",
    "barcode": "007",
    "caloriePer100g": 389.0,
    "servingSizeG": 40.0,
    "nutrient": [
      {"type":"Protein","unit":"g","value":3.4},
      {"type":"Carbs","unit":"g","value":4.1},
      {"type":"Fat","unit":"g","value":1.5}
    ]
  }'
```

#### Update food
`PUT /foods` (same JSON shape as POST)

```bash
curl -X PUT "http://127.0.0.1:18080/foods" \
  -H "Content-Type: application/json" \
  -d '{
    "id": "007",
    "name": "Oats UPDATED",
    "brand": "Demo",
    "barcode": "007",
    "caloriePer100g": 370.0,
    "servingSizeG": 40.0,
    "nutrient": []
  }'
```

#### Delete one food
`DELETE /foods?barcode=007`

```bash
curl -X DELETE "http://127.0.0.1:18080/foods?barcode=007"
```

---

### Meals

#### List meals
`GET /meals?offset=0&limit=50`

```bash
curl "http://127.0.0.1:18080/meals?offset=0&limit=50"
```

#### Find meals by name
`GET /meals/by_name?name=Lunch`

```bash
curl "http://127.0.0.1:18080/meals/by_name?name=Lunch"
```

#### Find meal by id
`GET /meals/by_id?id=10`

```bash
curl "http://127.0.0.1:18080/meals/by_id?id=10"
```

#### Find meals by date
`GET /meals/by_date?day=2&month=2&year=2026`

```bash
curl "http://127.0.0.1:18080/meals/by_date?day=2&month=2&year=2026"
```

#### Create meal
`POST /meals`

Payload (foodItems uses arrays in this implementation: `[["barcode", grams]]`):

```bash
curl -X POST "http://127.0.0.1:18080/meals" \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Lunch",
    "tsUtc": "2026-02-02T13:05:00Z",
    "foodItems": [["007", 50]]
  }'
```

#### Update meal
`PUT /meals`

```bash
curl -X PUT "http://127.0.0.1:18080/meals" \
  -H "Content-Type: application/json" \
  -d '{
    "id": 1,
    "name": "Dinner",
    "tsUtc": "2026-02-02T20:00:00Z",
    "foodItems": [["007", 40]]
  }'
```

#### Delete meal
`DELETE /meals?id=1`

```bash
curl -X DELETE "http://127.0.0.1:18080/meals?id=1"
```

#### Clear meals DB (dev/testing)
`DELETE /meals/clear`

```bash
curl -X DELETE "http://127.0.0.1:18080/meals/clear"
```

---

## Tests

### C++ unit tests (CTest)
If your top-level CMake enables tests, you can run:

```bash
cmake -S . -B build-ut -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON
cmake --build build-ut -j
ctest --test-dir build-ut --output-on-failure
```

### API integration tests (pytest)
Install dependencies:

```bash
python3 -m pip install -U pytest requests
```

Start the server (terminal 1):

```bash
export CC_FOODS_DB_PATH=/tmp/cc_test_foods.json
export CC_MEALS_DB_PATH=/tmp/cc_test_meals.json
./build/bin/cc_app
```

Run tests (terminal 2):

```bash
pytest -q tests/test_api
```

---

## Notes / Next improvements
- Add a non-interactive run mode for Docker/CI (no “press Enter”).
- Add Docker Compose with persistent volume + healthcheck.
- Add `/stats/day` and `/meals/by_range` endpoints (if needed for analytics).

