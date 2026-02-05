#include "clients/OpenFoodFactsClient.hpp"
#include "models/food.hpp"
#include "models/meal_log.hpp"
#include "models/nutrient.hpp"
#include "services/FoodService.hpp"
#include "storage/FoodRepository.hpp"
#include "storage/JsonFoodRepository.hpp"
#include "utils/Result.hpp"
#include "utils/date_time_utils.hpp"
#include <chrono>
#include <format>
#include <gtest/gtest.h>
#include <iterator>
#include <magic_enum.hpp>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using namespace cc::services;

class FoodServiceTest : public ::testing::Test {
protected:
  void SetUp() override { // runs BEFORE each TEST_F
  }

  void TearDown() override { // runs AFTER each TEST_F
                             // nothing to destroy //
  }
  std::string path_to_temp_db{"/tmp/cc_UT_test_db.json"};
  std::string wrong_path_to_temp_db{"/tmmp/cc_UT_test_db.json"};

  // helper functions and members visible to all TEST_F in this suite
};

TEST_F(FoodServiceTest, listFoods) {
  cc::clients::OpenFoodFactsClient client;
  std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
      std::make_shared<cc::storage::JsonFoodRepository>(path_to_temp_db);
  std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
      std::make_shared<cc::clients::OpenFoodFactsClient>(client);
  FoodService food_service{repo_shared_ptr, client_ptr};
  food_service.clear_data_base();
  EXPECT_EQ(food_service.listFoods().unwrap().size(), 0);
}
TEST_F(FoodServiceTest, listFoods_data_base_path_is_wrong) {
  cc::clients::OpenFoodFactsClient client;
  std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
      std::make_shared<cc::storage::JsonFoodRepository>(wrong_path_to_temp_db);
  std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
      std::make_shared<cc::clients::OpenFoodFactsClient>(client);
  FoodService food_service{repo_shared_ptr, client_ptr};
  ;
  EXPECT_EQ(food_service.clear_data_base().unwrap_error().code,
            cc::utils::ErrorCode::StorageError);
  EXPECT_EQ(food_service.listFoods().unwrap_error().code,
            cc::utils::ErrorCode::NotFound);
}
TEST_F(FoodServiceTest, addManualFood) {
  cc::clients::OpenFoodFactsClient client;
  std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
      std::make_shared<cc::storage::JsonFoodRepository>(path_to_temp_db);
  std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
      std::make_shared<cc::clients::OpenFoodFactsClient>(client);
  FoodService food_service{repo_shared_ptr, client_ptr};
  food_service.clear_data_base();
  EXPECT_EQ(food_service.listFoods().unwrap().size(), 0);
  cc::models::Food food;
  food.setId("00000");
  food.setName("minina");
  food.setBrand("Aicha");
  food.setBarcode("0707070");
  food.setCaloriesPer100g(200);
  food.setSource(cc::models::SOURCE::Manual);
  food.setImageUrl(std::string("https://example.com/granola.jpg"));
  food.setNutrients({{cc::models::NutrientType::Protein, 24, "g"}, {cc::models::NutrientType::Carbs, 100, "g"}});
  food_service.addManualFood(food);
  EXPECT_EQ(food_service.listFoods().unwrap().size(), 1);
}

TEST_F(FoodServiceTest, addManualFood_wrong_path_to_data_base) {
  cc::clients::OpenFoodFactsClient client;
  std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
      std::make_shared<cc::storage::JsonFoodRepository>(wrong_path_to_temp_db);
  std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
      std::make_shared<cc::clients::OpenFoodFactsClient>(client);
  FoodService food_service{repo_shared_ptr, client_ptr};
  cc::models::Food food;
  food.setId("00000");
  food.setName("minina");
  food.setBrand("Aicha");
  food.setBarcode("0707070");
  food.setCaloriesPer100g(200);
  food.setSource(cc::models::SOURCE::Manual);
  food.setImageUrl(std::string("https://example.com/granola.jpg"));
  food.setNutrients({{cc::models::NutrientType::Protein, 24, "g"}, {cc::models::NutrientType::Carbs, 100, "g"}});
  food_service.addManualFood(food);
  EXPECT_EQ(food_service.listFoods().unwrap_error().code,
            cc::utils::ErrorCode::NotFound);
  EXPECT_EQ(food_service.addManualFood(food).unwrap_error().code,
            cc::utils::ErrorCode::StorageError);
}

TEST_F(FoodServiceTest, updateFood) {
  cc::clients::OpenFoodFactsClient client;
  std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
      std::make_shared<cc::storage::JsonFoodRepository>(path_to_temp_db);
  std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
      std::make_shared<cc::clients::OpenFoodFactsClient>(client);
  FoodService food_service{repo_shared_ptr, client_ptr};
  food_service.clear_data_base();
  EXPECT_EQ(food_service.listFoods().unwrap().size(), 0);
  cc::models::Food food;
  food.setId("00000");
  food.setName("minina");
  food.setBrand("Aicha");
  food.setBarcode("0707070");
  food.setCaloriesPer100g(200);
  food.setSource(cc::models::SOURCE::Manual);
  food.setImageUrl(std::string("https://example.com/granola.jpg"));
  food.setNutrients({{cc::models::NutrientType::Protein, 24, "g"}, {cc::models::NutrientType::Carbs, 100, "g"}});
  food_service.addManualFood(food);
  EXPECT_EQ(food_service.listFoods().unwrap().size(), 1);
  food.setName("new name");
  food_service.updateFood(food);
  EXPECT_EQ(food_service.listFoods().unwrap()[0].name(), food.name());
  std::remove(path_to_temp_db.c_str());
}

TEST_F(FoodServiceTest, updateFood_wrong_path_to_data_base) {
  cc::clients::OpenFoodFactsClient client;
  std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
      std::make_shared<cc::storage::JsonFoodRepository>(wrong_path_to_temp_db);
  std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
      std::make_shared<cc::clients::OpenFoodFactsClient>(client);
  FoodService food_service{repo_shared_ptr, client_ptr};
  cc::models::Food food;
  food.setId("00000");
  food.setName("minina");
  food.setBrand("Aicha");
  food.setBarcode("0707070");
  food.setCaloriesPer100g(200);
  food.setSource(cc::models::SOURCE::Manual);
  food.setImageUrl(std::string("https://example.com/granola.jpg"));
  food.setNutrients({{cc::models::NutrientType::Protein, 24, "g"}, {cc::models::NutrientType::Carbs, 100, "g"}});
  EXPECT_EQ(food_service.addManualFood(food).unwrap_error().code,
            cc::utils::ErrorCode::StorageError);
  EXPECT_EQ(food_service.updateFood(food).unwrap_error().code,
            cc::utils::ErrorCode::StorageError);
}

TEST_F(FoodServiceTest, deleteFood) {
  cc::clients::OpenFoodFactsClient client;
  std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
      std::make_shared<cc::storage::JsonFoodRepository>(path_to_temp_db);
  std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
      std::make_shared<cc::clients::OpenFoodFactsClient>(client);
  FoodService food_service{repo_shared_ptr, client_ptr};
  food_service.clear_data_base();
  EXPECT_EQ(food_service.listFoods().unwrap().size(), 0);
  cc::models::Food food;
  food.setId("00000");
  food.setName("minina");
  food.setBrand("Aicha");
  food.setBarcode("0707070");
  food.setCaloriesPer100g(200);
  food.setSource(cc::models::SOURCE::Manual);
  food.setImageUrl(std::string("https://example.com/granola.jpg"));
  food.setNutrients({{cc::models::NutrientType::Protein, 24, "g"}, {cc::models::NutrientType::Carbs, 100, "g"}});
  food_service.addManualFood(food);
  EXPECT_EQ(food_service.listFoods().unwrap().size(), 1);
  EXPECT_EQ(food_service.listFoods().unwrap()[0].name(), food.name());
  EXPECT_EQ(food_service.listFoods().unwrap()[0].id(), food.id());
  food_service.deleteFood(food.id());
  std::remove(path_to_temp_db.c_str());
}

TEST_F(FoodServiceTest, deleteFood_wrong_id) {
  cc::clients::OpenFoodFactsClient client;
  std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
      std::make_shared<cc::storage::JsonFoodRepository>(path_to_temp_db);
  std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
      std::make_shared<cc::clients::OpenFoodFactsClient>(client);
  FoodService food_service{repo_shared_ptr, client_ptr};
  food_service.clear_data_base();
  EXPECT_EQ(food_service.listFoods().unwrap().size(), 0);
  cc::models::Food food;
  food.setId("00000");
  food.setName("minina");
  food.setBrand("Aicha");
  food.setBarcode("0707070");
  food.setCaloriesPer100g(200);
  food.setSource(cc::models::SOURCE::Manual);
  food.setImageUrl(std::string("https://example.com/granola.jpg"));
  food.setNutrients({{cc::models::NutrientType::Protein, 24, "g"}, {cc::models::NutrientType::Carbs, 100, "g"}});
  food_service.addManualFood(food);
  EXPECT_EQ(food_service.listFoods().unwrap().size(), 1);
  EXPECT_EQ(food_service.listFoods().unwrap()[0].name(), food.name());
  EXPECT_EQ(food_service.listFoods().unwrap()[0].id(), food.id());
  std::string wrong_id = std::format("{}+wrong", food.id());
  food_service.deleteFood(wrong_id);
  EXPECT_EQ(food_service.deleteFood(wrong_id).unwrap_error().code,
            cc::utils::ErrorCode::StorageError);
  EXPECT_EQ(food_service.listFoods().unwrap().size(), 1);
  std::remove(path_to_temp_db.c_str());
}

TEST_F(FoodServiceTest, getOrFetchByBarcode) {
  cc::clients::OpenFoodFactsClient client;
  std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
      std::make_shared<cc::storage::JsonFoodRepository>(path_to_temp_db);
  std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
      std::make_shared<cc::clients::OpenFoodFactsClient>(client);
  FoodService food_service{repo_shared_ptr, client_ptr};
  food_service.clear_data_base();
  std::string whey_protein_barcode("4250519647425");
  // Fetch from online data base  because food doesn't exist locally
  cc::utils::Result<cc::models::Food> whey_protein_food =
      food_service.getOrFetchByBarcode(whey_protein_barcode);
  EXPECT_EQ(whey_protein_food.unwrap().id(), whey_protein_barcode);

  // Fetch from local data base because it is already saved to local data base
  // food_service.addManualFood(kefir.unwrap());
  cc::utils::Result<cc::models::Food> food =
      food_service.getOrFetchByBarcode(whey_protein_barcode);
  EXPECT_EQ(food.unwrap().id(), whey_protein_barcode);
}

TEST_F(FoodServiceTest, getOrFetchByBarcode_wrong_id) {
  cc::clients::OpenFoodFactsClient client;
  std::shared_ptr<cc::storage::JsonFoodRepository> repo_shared_ptr =
      std::make_shared<cc::storage::JsonFoodRepository>(path_to_temp_db);
  std::shared_ptr<cc::clients::OpenFoodFactsClient> client_ptr =
      std::make_shared<cc::clients::OpenFoodFactsClient>(client);
  FoodService food_service{repo_shared_ptr, client_ptr};
  food_service.clear_data_base();
  std::string wrong_barcode("000000000000000");
  cc::utils::Result<cc::models::Food> food =
      food_service.getOrFetchByBarcode(wrong_barcode);
  EXPECT_EQ(food.unwrap_error().code, cc::utils::ErrorCode::NotFound);
}
