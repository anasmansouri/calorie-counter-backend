#!/bin/bash
#rm json_data_base.json
mkdir -p build
cd build
#rm -r *
#cmake .. 
#make 

#to run clang tidy to make static analysis
#cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_CLANG_TIDY=ON -DENABLE_COVERAGE=ON -DBUILD_TESTS=ON ..
cmake -DCMAKE_BUILD_TYPE=Debug  -DENABLE_COVERAGE=ON -DBUILD_TESTS=ON ..
cmake --build . -j 
ctest --output-on-failure
mkdir -p report
gcovr -r .. --filter '../src' --html --html-details -o report/coverage.html
#./bin/cc_test_models
#./bin/cc_test_models
./bin/cc_app
#gdb ./bin/cc_app
