# tests/CMakeLists.txt
include(FetchContent)

# Fetch GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(Threads REQUIRED)

#add_executable(cc_test_models test_models.cpp test_JsonMealRepository.cpp)
add_executable(cc_test_models
    test_models/test_meal_model.cpp
    test_models/test_nutrient.cpp
    test_models/test_daily_log.cpp
    test_models/test_food.cpp
    test_clients/test_OpenFoodFactsClient.cpp
    test_storage/test_JsonFoodRepository.cpp
    test_service/test_food_service.cpp
    )

target_include_directories(cc_test_models
    SYSTEM PUBLIC  ${CMAKE_SOURCE_DIR}/third_party
)

target_link_libraries(cc_test_models PRIVATE
  cc_utils
  cc_models
  cc_clients
  cc_storage
  cc_services
  #cc_api
  GTest::gtest_main
  Threads::Threads
)

# Auto-discover tests for CTest
include(GoogleTest)
gtest_discover_tests(cc_test_models)
