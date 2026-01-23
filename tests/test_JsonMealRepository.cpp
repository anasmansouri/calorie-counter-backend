#include <bits/chrono.h>
#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
#include <utility>
#include <vector>
#include "models/meal_log.hpp"
#include "models/food.hpp"
#include "storage/JsonMealRepository.hpp"
#include "utils/Result.hpp"
#include "utils/date_time_utils.hpp"

using namespace cc::storage;

class JsonMealRepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {                // runs BEFORE each TEST_F
    }
   void TearDown() override {             // runs AFTER each TEST_F
           // nothing to destroy                                //
    }

    // helper functions and members visible to all TEST_F in this suite
};

TEST_F(JsonMealRepositoryTest,initializition) {

    JsonMealRepository mealRepo("/home/anas/personal_projects/calorie-counter-backend/json_meal_data_base.json");
    cc::models::Food minina;
    minina.setId("2131654967498");
    minina.setName("minina");
    minina.setBrand(std::string("wlad 3icha l3arbi"));
    minina.setBarcode(std::string("2131654967498"));
    minina.setCaloriesPer100g(420.0);
    minina.setServingSizeG(40.0); // default serving size 40g
    minina.setSource(cc::models::SOURCE::Manual);
    minina.setImageUrl(std::string("https://example.com/granola.jpg"));

    cc::models::MealLog breakfast(cc::models::MEALNAME::Breakfast);
    breakfast.addFoodItem(minina.id(),10);
    EXPECT_NO_THROW(mealRepo.save(breakfast));
    }

TEST_F(JsonMealRepositoryTest,getById) {
    JsonMealRepository mealRepo("/home/anas/personal_projects/calorie-counter-backend/json_meal_data_base.json");
    cc::models::Food minina;
    minina.setId("2131654967498");
    minina.setName("minina");
    minina.setBrand(std::string("wlad 3icha l3arbi"));
    minina.setBarcode(std::string("2131654967498"));
    minina.setCaloriesPer100g(420.0);
    minina.setServingSizeG(40.0); // default serving size 40g
    minina.setSource(cc::models::SOURCE::Manual);
    minina.setImageUrl(std::string("https://example.com/granola.jpg"));

    cc::models::MealLog breakfast(cc::models::MEALNAME::Breakfast);
    // std::chrono::system_clock::time_point id_tsUtc_ = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
    std::chrono::system_clock::time_point id_tsUtc_ = std::chrono::system_clock::now();
    breakfast.setTime(id_tsUtc_);
    breakfast.setId("anas");
    breakfast.addFoodItem(minina.id(),10);
    EXPECT_NO_THROW(mealRepo.save(breakfast));
    cc::utils::Result<cc::models::MealLog> result_meal= mealRepo.getById("anas");
    std::cout<<"result meal id "<<result_meal.unwrap().id()<<std::endl;
    std::cout<<"breakfast id "<<breakfast.id()<<std::endl;
    EXPECT_EQ(result_meal.unwrap().id(), breakfast.id());
    EXPECT_EQ(result_meal.unwrap().getName(), breakfast.getName());
    }
/*

TEST_F(MealModelTest,setName) {
    breakfast.setName(MEALNAME::Lunch);
    EXPECT_EQ(breakfast.getName(),MEALNAME::Lunch);
    }

TEST_F(MealModelTest,setId) {
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    breakfast.setId(tp);
    EXPECT_EQ(breakfast.id(),tp);
    }

TEST_F(MealModelTest,addFoodItem) {
    cc::models::Food minina;
    minina.setId("2131654967498");
    minina.setName("minina");
    minina.setBrand(std::string("wlad 3icha l3arbi"));
    minina.setBarcode(std::string("2131654967498"));
    minina.setCaloriesPer100g(420.0);
    minina.setServingSizeG(40.0); // default serving size 40g
    minina.setSource("manual");
    minina.setImageUrl(std::string("https://example.com/granola.jpg"));
    breakfast.addFoodItem(minina.id(),50);

    EXPECT_EQ(breakfast.food_items()[0].first,minina.id());
    EXPECT_EQ(breakfast.food_items()[0].second,50);
    }

TEST_F(MealModelTest,removeFoodItem) {
    cc::models::Food minina;
    minina.setId("2131654967498");
    minina.setName("minina");
    minina.setBrand(std::string("wlad 3icha l3arbi"));
    minina.setBarcode(std::string("2131654967498"));
    minina.setCaloriesPer100g(420.0);
    minina.setServingSizeG(40.0); // default serving size 40g
    minina.setSource("manual");
    minina.setImageUrl(std::string("https://example.com/granola.jpg"));
    breakfast.addFoodItem(minina.id(),50);
    EXPECT_EQ(breakfast.food_items()[0].first,minina.id());
    EXPECT_EQ(breakfast.food_items()[0].second,50);

    EXPECT_EQ(breakfast.removeFoodItem(minina.id()),true);
    EXPECT_EQ(breakfast.food_items().size(),0);
    EXPECT_EQ(breakfast.removeFoodItem(minina.id()),false);
    }

TEST_F(MealModelTest,setFoodItems) {
    // food item 1
    cc::models::Food minina;
    minina.setId("2131654967498");
    minina.setName("minina");
    minina.setBrand(std::string("wlad 3icha l3arbi"));
    minina.setBarcode(std::string("2131654967498"));
    minina.setCaloriesPer100g(420.0);
    minina.setServingSizeG(40.0); // default serving size 40g
    minina.setSource("manual");
    minina.setImageUrl(std::string("https://example.com/granola.jpg"));

    // food item 1
    cc::models::Food minina_2;
    minina_2.setId("88877788888");
    minina_2.setName("minina_2");
    minina_2.setBrand(std::string("factory_xx"));
    minina_2.setBarcode(std::string("88877788888"));
    minina_2.setCaloriesPer100g(420.0);
    minina_2.setServingSizeG(40.0); // default serving size 40g
    minina_2.setSource("manual");
    minina_2.setImageUrl(std::string("https://example.com/granola.jpg"));

    std::vector<std::pair<std::string, double>> food_items{{minina.id(),100},{minina_2.id(),50}};

    breakfast.setFoodItems(food_items);
    EXPECT_EQ(breakfast.food_items()[0].first,minina.id());
    EXPECT_EQ(breakfast.food_items()[0].second,100);

    EXPECT_EQ(breakfast.food_items()[1].first,minina_2.id());
    EXPECT_EQ(breakfast.food_items()[1].second,50);
    }

    
*/
/*
TEST(Math, Multiply) {
    EXPECT_EQ(multiply(2, 4), 8);
}

TEST(Math, Divide) {
    EXPECT_DOUBLE_EQ(divide(10, 2), 5.0);
    EXPECT_THROW(divide(10, 0), std::invalid_argument);
}

TEST(Math, Average) {
    std::vector<int> nums = {1, 2, 3, 4};
    EXPECT_DOUBLE_EQ(average(nums), 2.5);
}
*/
