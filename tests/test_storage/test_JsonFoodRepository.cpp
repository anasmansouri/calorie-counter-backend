#include "models/food.hpp"
#include "models/nutrient.hpp"
#include "storage/JsonFoodRepository.hpp"
#include "utils/Result.hpp"
#include <cstdio>
#include <format>
#include <gtest/gtest.h>
#include <pstl/glue_algorithm_defs.h>
#include <string>
#include <vector>

using namespace cc::storage;

class JsonFoodRepositoryTest : public ::testing::Test {
protected:
  void SetUp() override { // runs BEFORE each TEST_F
    food.setId("00000");
    food.setName("minina");
    food.setBrand("Aicha");
    food.setBarcode("0707070");
    food.setCaloriesPer100g(200);
    food.setSource(cc::models::SOURCE::Manual);
    food.setImageUrl(std::string("https://example.com/granola.jpg"));
    food.setNutrients({{cc::models::NutrientType::Protein, 24, "g"}, {cc::models::NutrientType::Carbs, 100, "g"}});
  }

  void TearDown() override { // runs AFTER each TEST_F
                             // nothing to destroy //
  }


  // helper functions and members visible to all TEST_F in this suite
  
  std::string path_to_temp_db{"/tmp/cc_UT_test_db.json"};
  std::string wrong_path_to_temp_db{"/tmmp/cc_UT_test_db.json"};

  JsonFoodRepository repo{path_to_temp_db};
  cc::models::Food food;
};

TEST_F(JsonFoodRepositoryTest, save_and_data_base_empty) {

  JsonFoodRepository repo_temp{path_to_temp_db};
  EXPECT_NO_THROW(repo_temp.save(food));
  EXPECT_EQ(repo_temp.getById_or_Barcode(food.id()).unwrap().id(), food.id());
  EXPECT_EQ(repo_temp.getById_or_Barcode(food.id()).unwrap().name(),
            food.name());
  EXPECT_EQ(repo_temp.getById_or_Barcode(food.id()).unwrap().brand(),
            food.brand());
  EXPECT_EQ(repo_temp.getById_or_Barcode(food.id()).unwrap().barcode(),
            food.barcode());
  EXPECT_EQ(repo_temp.getById_or_Barcode(food.id()).unwrap().source(),
            food.source());
  EXPECT_EQ(repo_temp.getById_or_Barcode(food.id()).unwrap().imageUrl(),
            food.imageUrl());
  EXPECT_EQ(repo_temp.getById_or_Barcode(food.id()).unwrap().caloriesPer100g(),
            food.caloriesPer100g());
  EXPECT_EQ(repo_temp.getById_or_Barcode(food.id()).value.value().totalKcal(100),
            food.totalKcal(100));
  EXPECT_EQ(repo_temp.getById_or_Barcode(food.id()).unwrap().to_string(),
            food.to_string());
  EXPECT_EQ(
      repo_temp.getById_or_Barcode(food.id()).unwrap().nutrients()[0].type(),
      food.nutrients()[0].type());
  EXPECT_EQ(
      repo_temp.getById_or_Barcode(food.id()).unwrap().nutrients()[1].type(),
      food.nutrients()[1].type());
  // remove file , so it is always empty data base
  std::remove(path_to_temp_db.c_str());
}
TEST_F(JsonFoodRepositoryTest, save_wrong_path_to_data_base) {

  JsonFoodRepository repo_temp{wrong_path_to_temp_db};
  EXPECT_EQ(repo_temp.save(food).unwrap_error().code,
            cc::utils::ErrorCode::StorageError);
  EXPECT_EQ(repo_temp.getById_or_Barcode(food.id()).unwrap_error().code,
            cc::utils::ErrorCode::NotFound);
}

TEST_F(JsonFoodRepositoryTest, save) {
  EXPECT_NO_THROW(repo.save(food));
  // no error so error.has_value() is expected false
  EXPECT_FALSE(repo.save(food).error.has_value());
}
TEST_F(JsonFoodRepositoryTest, getById_or_Barcode) {

  EXPECT_NO_THROW(repo.save(food));
  EXPECT_EQ(repo.getById_or_Barcode(food.id()).unwrap().id(), food.id());
  EXPECT_EQ(repo.getById_or_Barcode(food.id()).unwrap().name(), food.name());
  EXPECT_EQ(repo.getById_or_Barcode(food.id()).unwrap().brand(), food.brand());
  EXPECT_EQ(repo.getById_or_Barcode(food.id()).unwrap().barcode(),
            food.barcode());
  EXPECT_EQ(repo.getById_or_Barcode(food.id()).unwrap().source(),
            food.source());
  EXPECT_EQ(repo.getById_or_Barcode(food.id()).unwrap().imageUrl(),
            food.imageUrl());
  EXPECT_EQ(repo.getById_or_Barcode(food.id()).unwrap().caloriesPer100g(),
            food.caloriesPer100g());
  EXPECT_EQ(repo.getById_or_Barcode(food.id()).value.value().totalKcal(100),
            food.totalKcal(100));
  EXPECT_EQ(repo.getById_or_Barcode(food.id()).unwrap().to_string(),
            food.to_string());
  EXPECT_EQ(repo.getById_or_Barcode(food.id()).unwrap().nutrients()[0].type(),
            food.nutrients()[0].type());
  EXPECT_EQ(repo.getById_or_Barcode(food.id()).unwrap().nutrients()[1].type(),
            food.nutrients()[1].type());
}

TEST_F(JsonFoodRepositoryTest, getById_or_Barcode_ITEM_NOT_FOUND) {
  EXPECT_NO_THROW(repo.save(food));
  std::string wrong_id = std::format("{}wrong", food.id());
  EXPECT_EQ(repo.getById_or_Barcode(wrong_id).unwrap_error().code,
            cc::utils::ErrorCode::NotFound);
}

TEST_F(JsonFoodRepositoryTest, list) {
  JsonFoodRepository repo_temp{path_to_temp_db};
  EXPECT_NO_THROW(repo_temp.save(food));
  cc::utils::Result<std::vector<cc::models::Food>> food_list = repo_temp.list();
  EXPECT_EQ(food_list.unwrap().size(), 1);
  EXPECT_EQ(food_list.unwrap()[0].id(), food.id());
  EXPECT_EQ(food_list.unwrap()[0].name(), food.name());
  EXPECT_EQ(food_list.unwrap()[0].brand(), food.brand());
  EXPECT_EQ(food_list.unwrap()[0].barcode(), food.barcode());
  EXPECT_EQ(food_list.unwrap()[0].source(), food.source());
  EXPECT_EQ(food_list.unwrap()[0].imageUrl(), food.imageUrl());
  EXPECT_EQ(food_list.unwrap()[0].caloriesPer100g(), food.caloriesPer100g());
  EXPECT_EQ(food_list.unwrap()[0].totalKcal(100), food.totalKcal(100));
  EXPECT_EQ(food_list.unwrap()[0].to_string(), food.to_string());
  EXPECT_EQ(food_list.unwrap()[0].nutrients()[0].type(),
            food.nutrients()[0].type());
  EXPECT_EQ(food_list.unwrap()[0].nutrients()[1].type(),
            food.nutrients()[1].type());
  std::remove(path_to_temp_db.c_str());
}
TEST_F(JsonFoodRepositoryTest, list_cant_open_data_base) {
  // wrong_path
  JsonFoodRepository repo_temp{wrong_path_to_temp_db};
  EXPECT_EQ(repo_temp.save(food).unwrap_error().code,
            cc::utils::ErrorCode::StorageError);
  EXPECT_EQ(repo_temp.list().unwrap_error().code,
            cc::utils::ErrorCode::NotFound);
}

TEST_F(JsonFoodRepositoryTest, remove) {

  JsonFoodRepository repo_temp{path_to_temp_db};

  EXPECT_NO_THROW(repo_temp.save(food));
  cc::utils::Result<std::vector<cc::models::Food>> food_list = repo_temp.list();
  EXPECT_EQ(food_list.unwrap().size(), 1);
  EXPECT_EQ(food_list.unwrap()[0].id(), food.id());
  EXPECT_EQ(food_list.unwrap()[0].name(), food.name());
  EXPECT_NO_THROW(repo_temp.remove(food.id()));
  food_list = repo_temp.list();
  EXPECT_EQ(food_list.unwrap().size(), 0);
  std::remove(path_to_temp_db.c_str());
}

TEST_F(JsonFoodRepositoryTest, remove_a_non_existing_item) {
  JsonFoodRepository repo_temp{path_to_temp_db};
  EXPECT_NO_THROW(repo_temp.save(food));
  std::string wrong_id = std::format("{}wrong", food.id());
  EXPECT_EQ(repo.remove(wrong_id).unwrap_error().code,
            cc::utils::ErrorCode::NotFound);
  std::remove(path_to_temp_db.c_str());
}
TEST_F(JsonFoodRepositoryTest, remove_wrong_path_to_data_base) {
  // wrong_path
  JsonFoodRepository repo_temp{wrong_path_to_temp_db};
  EXPECT_EQ(repo_temp.save(food).unwrap_error().code,
            cc::utils::ErrorCode::StorageError);
  EXPECT_EQ(repo_temp.list().unwrap_error().code,
            cc::utils::ErrorCode::NotFound);
  EXPECT_EQ(repo_temp.remove(food.id()).unwrap_error().code,
            cc::utils::ErrorCode::NotFound);
}

TEST_F(JsonFoodRepositoryTest, upsert) {
  JsonFoodRepository repo_temp{path_to_temp_db};

  EXPECT_NO_THROW(repo_temp.save(food));
  EXPECT_EQ(repo_temp.getById_or_Barcode(food.id()).unwrap().id(), food.id());
  food.setName("anas");
  EXPECT_NO_THROW(repo_temp.upsert(food));
  EXPECT_EQ(repo_temp.getById_or_Barcode(food.id()).unwrap().id(), food.id());
  std::remove(path_to_temp_db.c_str());
}
TEST_F(JsonFoodRepositoryTest, clear) {
  JsonFoodRepository repo_temp{path_to_temp_db};

  EXPECT_NO_THROW(repo_temp.save(food));
  cc::utils::Result<std::vector<cc::models::Food>> food_list = repo_temp.list();
  EXPECT_EQ(food_list.unwrap().size(), 1);
  EXPECT_EQ(repo_temp.getById_or_Barcode(food.id()).unwrap().id(), food.id());
  repo_temp.clear();
  food_list = repo_temp.list();
  EXPECT_EQ(food_list.unwrap().size(), 0);
}
