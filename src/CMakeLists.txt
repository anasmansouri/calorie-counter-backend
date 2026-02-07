# to use enum as string with implementing to string functions
include(FetchContent)

FetchContent_Declare(
  magic_enum
  GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
  GIT_TAG v0.9.6
)

FetchContent_MakeAvailable(magic_enum)

# Utils module
add_library(cc_utils
    utils/Result.hpp utils/Result.cpp
    utils/HttpClient.hpp utils/HttpClient.cpp
    utils/Json_utils.hpp utils/Json_utils.cpp
    utils/common_functions.cpp utils/common_functions.hpp 
)
target_include_directories(
    cc_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(cc_utils
    SYSTEM PUBLIC  ${CMAKE_SOURCE_DIR}/third_party
)

#target_link_libraries(cc_utils PUBLIC magic_enum::magic_enum)
# Models module
add_library(cc_models
    models/food.hpp models/food.cpp
    models/nutrient.hpp models/nutrient.cpp
    models/meal_log.hpp models/meal_log.cpp
    models/daily_log.hpp models/daily_log.cpp
    models/DTOs.hpp
)
target_include_directories(
    cc_models PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(cc_models
    SYSTEM PUBLIC  ${CMAKE_SOURCE_DIR}/third_party
)
target_link_libraries(cc_models PUBLIC magic_enum::magic_enum)
target_link_libraries(cc_utils PUBLIC cc_models)
# Clients module
add_library(cc_clients
    clients/OpenFoodFactsClient.cpp clients/OpenFoodFactsClient.hpp
)

target_include_directories(cc_clients
    SYSTEM PUBLIC  ${CMAKE_SOURCE_DIR}/third_party
)

# libcurl library 
find_package(CURL REQUIRED)
#target_include_directories(cc_clients PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(cc_clients PRIVATE ${CURL_LIBRARIES})

target_include_directories(
    cc_clients PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cc_clients PUBLIC cc_utils)



# Storage module
add_library(cc_storage
    storage/FoodRepository.hpp
    storage/JsonFoodRepository.cpp storage/JsonFoodRepository.hpp
    storage/JsonMealRepository.cpp storage/JsonMealRepository.hpp
    storage/SqliteFoodRepository.cpp storage/SqliteFoodRepository.hpp
)
target_include_directories(
    cc_storage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(
    cc_storage SYSTEM PUBLIC  ${CMAKE_SOURCE_DIR}/third_party
)
target_link_libraries(cc_storage PUBLIC cc_models cc_utils)

# Services module
add_library(cc_services
    services/FoodService.cpp services/FoodService.hpp
    services/MealService.cpp services/MealService.hpp
    services/AuthService.cpp services/AuthService.hpp
    services/UserService.cpp services/UserService.hpp
)
target_include_directories(
    cc_services PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_include_directories(
    cc_services SYSTEM PUBLIC  ${CMAKE_SOURCE_DIR}/third_party
)
target_link_libraries(cc_services PUBLIC cc_storage cc_clients cc_models cc_utils)

# API module
add_library(cc_api
    api/Server.cpp api/Server.hpp
)
target_include_directories(cc_api PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cc_api PUBLIC cc_services cc_utils)
#######################



# Crow 
include(FetchContent)
FetchContent_Declare(
  crow
  GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
  GIT_TAG v1.2.0           # pin a tag or commit
  GIT_SHALLOW TRUE         # shallow clone (smaller)
  GIT_PROGRESS TRUE
  UPDATE_DISCONNECTED TRUE # don’t auto-update once downloaded
)
FetchContent_MakeAvailable(crow)
find_package(Threads REQUIRED)

# Main executable
add_executable(cc_app main.cpp)
target_link_libraries(cc_app PRIVATE cc_api ${CURL_LIBRARIES})
target_link_libraries(cc_app PRIVATE Crow::Crow)
target_link_libraries(cc_utils PUBLIC Crow::Crow)
target_link_libraries(cc_app PRIVATE cc_api)
target_link_libraries(cc_app PRIVATE Threads::Threads)
