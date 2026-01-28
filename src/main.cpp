
#include "clients/OpenFoodFactsClient.hpp"
#include "models/food.hpp"
#include "models/nutrient.hpp"
#include "nlohmann/json.hpp"
#include "services/FoodService.hpp"
#include "storage/FoodRepository.hpp"
#include "storage/JsonFoodRepository.hpp"
#include "utils/HttpClient.hpp"
#include "utils/Result.hpp"
#include "utils/common_functions.hpp"
#include <api/Server.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
int main() {

  const char *env_db = std::getenv("CC_DB_PATH");
  std::string db_path = (env_db && std::string(env_db).size() > 0)
                            ? std::string(env_db)
                            : cc::utils::default_db_path();

  cc::utils::ensure_db_file_exists(db_path);
  auto repo_shared_ptr =
      std::make_shared<cc::storage::JsonFoodRepository>(db_path);

  cc::clients::OpenFoodFactsClient client;
  std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
      std::make_shared<cc::clients::OpenFoodFactsClient>(client);
  cc::services::FoodService food_service{repo_shared_ptr, client_ptr};

  cc::api::Server server(
      18080, std::make_shared<cc::services::FoodService>(food_service));
  // server.setupRoutes();
  server.start();
  std::cout << "serving running , click Enter to stop it : " << std::endl;

  std::string line;
  std::getline(std::cin, line); // press Enter to stop
  server.stop();

  return 0;
}
