#!/usr/bin/env bash
set -e

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug  -DENABLE_COVERAGE=ON -DBUILD_TESTS=ON ..
cmake --build . -j
ctest --output-on-failure
mkdir -p report
gcovr -r .. --filter '../src' --html --html-details -o report/coverage.html
find .. -name "*.gcov" -delete
echo "UT OK"
