#include <chrono>
#include <format>
#include <gtest/gtest.h>
#include <pstl/glue_algorithm_defs.h>
#include <string>

#include "models/food.hpp"
#include "models/meal_log.hpp"
#include "storage/JsonMealRepository.hpp"
#include "utils/Result.hpp"
#include <cstdio>
#include <iostream>
#include <utility>
#include <vector>

using namespace cc::storage;

class JsonMealRepositoryTest : public ::testing::Test {
protected:
  void SetUp() override { // runs BEFORE each TEST_F
   
  meal.setName(cc::models::MEALNAME::Breakfast);
  meal.setTime(std::chrono::system_clock::now());
  /// food
  food.setId("2131654967498");
  food.setName("food");
  food.setBrand(std::string("wlad 3icha l3arbi"));
  food.setBarcode(std::string("2131654967498"));
  food.setCaloriesPer100g(420.0);
  food.setServingSizeG(40.0); // default serving size 40g
  food.setSource(cc::models::SOURCE::Manual);
  food.setImageUrl(std::string("https://example.com/granola.jpg"));
  ///////////////////////
  meal.addFoodItem(food.id(), food.servingSizeG().value());
  
  }

  void TearDown() override { // runs AFTER each TEST_F
                             // nothing to destroy //

    // std::remove(
    //    "/home/anas/personal_projects/calorie-counter-backend/json_data_base_meals.json");
  }

  // helper functions and members visible to all TEST_F in this suite
  cc::models::Food food;
  std::string path_to_meal_temp_db{"/tmp/cc_UT_test_meal_db.json"};
  std::string wrong_path_to_meal_temp_db{"/tmmp/cc_UT_test_meal_db.json"};
  cc::models::MealLog meal;
  JsonMealRepository repo{path_to_meal_temp_db};
  
};

TEST_F(JsonMealRepositoryTest, save_and_data_base_empty) {

  JsonMealRepository repo_temp{path_to_meal_temp_db};
  cc::models::MealLog lunch;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  cc::models::MealLog breakfast;

  breakfast.setName(cc::models::MEALNAME::Breakfast);
  lunch.setName(cc::models::MEALNAME::Lunch);
  // meal.setId(700);
  breakfast.setTime(std::chrono::system_clock::now());
  lunch.setTime(std::chrono::system_clock::now());
  /// food
  food.setId("2131654967498");
  food.setName("food");
  food.setBrand(std::string("wlad 3icha l3arbi"));
  food.setBarcode(std::string("2131654967498"));
  food.setCaloriesPer100g(420.0);
  food.setServingSizeG(40.0); // default serving size 40g
  food.setSource(cc::models::SOURCE::Manual);
  food.setImageUrl(std::string("https://example.com/granola.jpg"));
  ///////////////////////
  breakfast.addFoodItem(food.id(), food.servingSizeG().value());
  lunch.addFoodItem(food.id(), food.servingSizeG().value());
  ////////////////////////////////

  EXPECT_NO_THROW(repo_temp.save(breakfast));
  EXPECT_EQ(repo_temp.getById(breakfast.id()).value.value().id(),
            breakfast.id());
  EXPECT_EQ(repo_temp.getById(breakfast.id()).unwrap().getName(),
            breakfast.getName());
  EXPECT_EQ(repo_temp.getById(breakfast.id()).unwrap().gettime(),
            breakfast.gettime());
  EXPECT_EQ(repo_temp.getById(breakfast.id()).unwrap().food_items()[0].first,
            food.id());
  EXPECT_EQ(repo_temp.getById(breakfast.id()).unwrap().food_items()[0].second,
            food.servingSizeG().value());

  EXPECT_NO_THROW(repo_temp.save(lunch));
  std::remove("/home/anas/personal_projects/calorie-counter-backend/json_data_base_meals_im_testing_meals_id.json");
}


///////////////testing sync meals id /////////////////


TEST_F(JsonMealRepositoryTest, test_sync_meals_id) {
  JsonMealRepository repo_temp{path_to_meal_temp_db};
  repo_temp.sync_meals_id();
  

  cc::models::MealLog::next_id_=0;
  cc::models::MealLog breakfast;
  cc::models::MealLog lunch;

  breakfast.setName(cc::models::MEALNAME::Breakfast);
  lunch.setName(cc::models::MEALNAME::Lunch);
  // meal.setId(700);
  breakfast.setTime(std::chrono::system_clock::now());
  lunch.setTime(std::chrono::system_clock::now());
  /// food
  food.setId("2131654967498");
  food.setName("food");
  food.setBrand(std::string("wlad 3icha l3arbi"));
  food.setBarcode(std::string("2131654967498"));
  food.setCaloriesPer100g(420.0);
  food.setServingSizeG(40.0); // default serving size 40g
  food.setSource(cc::models::SOURCE::Manual);
  food.setImageUrl(std::string("https://example.com/granola.jpg"));
  ///////////////////////
  breakfast.addFoodItem(food.id(), food.servingSizeG().value());
  lunch.addFoodItem(food.id(), food.servingSizeG().value());
  ////////////////////////////////

  EXPECT_NE(breakfast.id(), lunch.id());
  EXPECT_NO_THROW(repo_temp.save(breakfast));
  EXPECT_NO_THROW(repo_temp.save(lunch));
  repo_temp.sync_meals_id();
  cc::models::MealLog dinner(cc::models::MEALNAME::Dinner);
  EXPECT_GT(dinner.id(),breakfast.id());
  repo_temp.save(dinner);
  
  std::remove(path_to_meal_temp_db.c_str());
}



///////////////testing sync meals id /////////////////



TEST_F(JsonMealRepositoryTest, save_wrong_path_to_data_base) {

    JsonMealRepository repo_temp{wrong_path_to_meal_temp_db};
    EXPECT_EQ(repo_temp.save(meal).unwrap_error().code,
cc::utils::ErrorCode::StorageError);
    EXPECT_EQ(repo_temp.getById(meal.id()).unwrap_error().code,
cc::utils::ErrorCode::NotFound);
}
TEST_F(JsonMealRepositoryTest, save) {
    JsonMealRepository repo_temp{path_to_meal_temp_db};
    EXPECT_NO_THROW(repo_temp.save(meal));
    // no error so error.has_value() is expected false
    EXPECT_FALSE(repo_temp.save(meal).error.has_value());
    std::remove(path_to_meal_temp_db.c_str());
}
TEST_F(JsonMealRepositoryTest, getById) {
    JsonMealRepository repo_temp{path_to_meal_temp_db};
    EXPECT_NO_THROW(repo_temp.save(meal));
    EXPECT_EQ(repo_temp.getById(meal.id()).value.value().id(), meal.id());
    EXPECT_EQ(repo_temp.getById(meal.id()).unwrap().getName(), meal.getName());
    EXPECT_EQ(repo_temp.getById(meal.id()).unwrap().gettime(), meal.gettime());
    EXPECT_EQ(repo_temp.getById(meal.id()).unwrap().food_items()[0].first,
food.id()); EXPECT_EQ(repo_temp.getById(meal.id()).unwrap().food_items()[0].second,
food.servingSizeG().value());
    std::remove(path_to_meal_temp_db.c_str());
}

TEST_F(JsonMealRepositoryTest, getById_ITEM_NOT_FOUND) {
    JsonMealRepository repo_temp{path_to_meal_temp_db};
    EXPECT_NO_THROW(repo_temp.save(meal));
    int wrong_id = meal.id()+1;
    EXPECT_EQ(repo.getById(wrong_id).unwrap_error().code,
cc::utils::ErrorCode::NotFound);
    std::remove(path_to_meal_temp_db.c_str());
}


TEST_F(JsonMealRepositoryTest, list) {
    JsonMealRepository repo_temp{path_to_meal_temp_db};
    EXPECT_NO_THROW(repo_temp.save(meal));
    cc::utils::Result<std::vector<cc::models::MealLog>> meal_list =
repo_temp.list(); EXPECT_EQ(meal_list.unwrap().size(), 1);
    EXPECT_EQ(meal_list.unwrap()[0].id(), meal.id());
    EXPECT_EQ(meal_list.unwrap()[0].getName(), meal.getName());
    EXPECT_EQ(meal_list.unwrap()[0].gettime(), meal.gettime());

    EXPECT_EQ(meal_list.unwrap()[0].food_items()[0].first, food.id());
    EXPECT_EQ(meal_list.unwrap()[0].food_items()[0].second,
food.servingSizeG().value());


    //EXPECT_EQ(food_list.unwrap()[0].barcode(), food.barcode());
    //EXPECT_EQ(food_list.unwrap()[0].source(), food.source());
    //EXPECT_EQ(food_list.unwrap()[0].imageUrl(), food.imageUrl());
    //EXPECT_EQ(food_list.unwrap()[0].caloriesPer100g(),
//food.caloriesPer100g());
    //EXPECT_EQ(food_list.unwrap()[0].totalKcal(), food.totalKcal());
    //EXPECT_EQ(food_list.unwrap()[0].to_string(), food.to_string());
    //EXPECT_EQ(food_list.unwrap()[0].nutrients()[0].name(),
//food.nutrients()[0].name());
    //EXPECT_EQ(food_list.unwrap()[0].nutrients()[1].name(),
//food.nutrients()[1].name());

    std::remove(path_to_meal_temp_db.c_str());
}


TEST_F(JsonMealRepositoryTest, list_cant_open_data_base) {
    // wrong_path
    JsonMealRepository repo_temp{wrong_path_to_meal_temp_db};
    EXPECT_EQ(repo_temp.save(meal).unwrap_error().code,
cc::utils::ErrorCode::StorageError);
    EXPECT_EQ(repo_temp.list().unwrap_error().code,
cc::utils::ErrorCode::NotFound);
}

TEST_F(JsonMealRepositoryTest, remove) {

    JsonMealRepository repo_temp{path_to_meal_temp_db};

    EXPECT_NO_THROW(repo_temp.save(meal));
    cc::utils::Result<std::vector<cc::models::MealLog>> meal_list =
repo_temp.list(); EXPECT_EQ(meal_list.unwrap().size(), 1);
    EXPECT_EQ(meal_list.unwrap()[0].id(), meal.id());
    EXPECT_EQ(meal_list.unwrap()[0].getName(), meal.getName());
    EXPECT_NO_THROW(repo_temp.remove(meal.id()));
    meal_list = repo_temp.list();
    EXPECT_EQ(meal_list.unwrap().size(), 0);
    std::remove(wrong_path_to_meal_temp_db.c_str());
}

TEST_F(JsonMealRepositoryTest, remove_a_non_existing_item) {
    JsonMealRepository repo_temp{path_to_meal_temp_db};
    EXPECT_NO_THROW(repo_temp.save(meal));
    int wrong_id = meal.id()+1;
    EXPECT_EQ(repo.remove(wrong_id).unwrap_error().code,
cc::utils::ErrorCode::NotFound);
    std::remove(path_to_meal_temp_db.c_str());
}

TEST_F(JsonMealRepositoryTest, remove_wrong_path_to_data_base) {
    // wrong_path
    JsonMealRepository repo_temp{
        wrong_path_to_meal_temp_db};
    EXPECT_EQ(repo_temp.save(meal).unwrap_error().code,
cc::utils::ErrorCode::StorageError);
    EXPECT_EQ(repo_temp.list().unwrap_error().code,
cc::utils::ErrorCode::NotFound);
    EXPECT_EQ(repo_temp.remove(meal.id()).unwrap_error().code,
cc::utils::ErrorCode::NotFound);
}

TEST_F(JsonMealRepositoryTest, upsert) {
    JsonMealRepository
repo_temp{path_to_meal_temp_db};

    
    EXPECT_NO_THROW(repo_temp.save(meal));
    EXPECT_EQ(repo_temp.getById(meal.id()).unwrap().id(), meal.id());
    meal.setName(cc::models::MEALNAME::Dinner);
    EXPECT_NO_THROW(repo_temp.upsert(meal));
    EXPECT_EQ(repo_temp.getById(meal.id()).unwrap().id(), meal.id());
    std::remove(path_to_meal_temp_db.c_str());
}

TEST_F(JsonMealRepositoryTest, upsert_wrong_path) {
    JsonMealRepository
repo_temp{wrong_path_to_meal_temp_db};

    EXPECT_EQ(repo_temp.upsert(meal).unwrap_error().code,
cc::utils::ErrorCode::StorageError);
}

TEST_F(JsonMealRepositoryTest, clear) {
    JsonMealRepository
repo_temp{path_to_meal_temp_db};

    EXPECT_NO_THROW(repo_temp.save(meal));
    cc::utils::Result<std::vector<cc::models::MealLog>> meal_list =
repo_temp.list(); EXPECT_EQ(meal_list.unwrap().size(), 1);
    EXPECT_EQ(repo_temp.getById(meal.id()).unwrap().id(), meal.id());
    repo_temp.clear();
    meal_list = repo_temp.list();
    EXPECT_EQ(meal_list.unwrap().size(), 0);
    std::remove(path_to_meal_temp_db.c_str());
}
