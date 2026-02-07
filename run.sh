#!/usr/bin/env bash
set -e

: "${CC_DB_PATH:=/tmp/cc_foods.json}"
: "${CC_MEALS_DB_PATH:=/tmp/cc_meals.json}"
export CC_DB_PATH CC_MEALS_DB_PATH

./build/bin/cc_app
