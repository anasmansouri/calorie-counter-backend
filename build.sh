#!/usr/bin/env bash
set -e

mkdir -p build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
echo "Build OK: build/bin/cc_app"
