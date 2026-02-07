
#include <bits/stdc++.h>

#include <api/Server.hpp>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <cstdio>   // fileno
#include <unistd.h> // isatty
#include "clients/OpenFoodFactsClient.hpp"
#include "services/FoodService.hpp"
#include "services/MealService.hpp"
#include "storage/JsonFoodRepository.hpp"
#include "storage/JsonMealRepository.hpp"
#include "utils/common_functions.hpp"
int main() {
  // setup data base
  const char* env_meals_db = std::getenv("CC_MEALS_DB_PATH");
  std::string meal_db_path =
      (env_meals_db && std::string(env_meals_db).size() > 0)
          ? std::string(env_meals_db)
          : cc::utils::default_meals_db_path();

  cc::utils::ensure_db_file_exists(meal_db_path);
  const char* env_food_db = std::getenv("CC_FOODS_DB_PATH");
  std::string food_db_path =
      (env_food_db && std::string(env_food_db).size() > 0)
          ? std::string(env_food_db)
          : cc::utils::default_food_db_path();

  ////////////////////
  cc::utils::ensure_db_file_exists(food_db_path);
  auto food_repo_shared_ptr =
      std::make_shared<cc::storage::JsonFoodRepository>(food_db_path);
  auto meal_repo_shared_ptr =
      std::make_shared<cc::storage::JsonMealRepository>(meal_db_path);

  cc::clients::OpenFoodFactsClient client;
  std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
      std::make_shared<cc::clients::OpenFoodFactsClient>(client);
  cc::services::FoodService food_service{food_repo_shared_ptr, client_ptr};
  cc::services::MealService meal_service{meal_repo_shared_ptr};

  cc::api::Server server(
      18080, std::make_shared<cc::services::FoodService>(food_service),
      std::make_shared<cc::services::MealService>(meal_service));
  server.start();

  bool interactive = ::isatty(fileno(stdin));
  if (interactive) {
    std::cout << "Press Enter to stop...\n";
    std::cin.get();
  } else {
    // Docker/CI: keep running until SIGTERM/SIGINT
    std::this_thread::sleep_for(std::chrono::hours(24 * 365 * 10));
  }
  server.stop();

  return 0;
}
