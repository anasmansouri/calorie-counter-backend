#include "models/food.hpp"
#include "models/meal_log.hpp"
#include "utils/date_time_utils.hpp"
#include <chrono>
#include <gtest/gtest.h>
#include <iterator>
#include <magic_enum.hpp>
#include <string>
#include <utility>
#include <vector>

using namespace cc::models;

class MealModelTest : public ::testing::Test {
  protected:
    void SetUp() override { // runs BEFORE each TEST_F
        meal.setName(MEALNAME::Breakfast);
    }

    void TearDown() override { // runs AFTER each TEST_F
                               // nothing to destroy                                //
    }

    // helper functions and members visible to all TEST_F in this suite
    MealLog meal;
};

TEST_F(MealModelTest, initializition) {
    MealLog lunch(MEALNAME::Lunch);
    MealLog breakfast(MEALNAME::Breakfast);
    EXPECT_EQ(lunch.getName(), MEALNAME::Lunch);
    EXPECT_EQ(breakfast.getName(), MEALNAME::Breakfast);
}

TEST_F(MealModelTest, setName) {
    meal.setName(MEALNAME::Lunch);
    EXPECT_EQ(meal.getName(), MEALNAME::Lunch);
}

TEST_F(MealModelTest, setId) {
    // std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    meal.setId(1);
    EXPECT_EQ(meal.id(), 1);
}

TEST_F(MealModelTest, addFoodItem) {
    cc::models::Food minina;
    minina.setId("2131654967498");
    minina.setName("minina");
    minina.setBrand(std::string("wlad 3icha l3arbi"));
    minina.setBarcode(std::string("2131654967498"));
    minina.setCaloriesPer100g(420.0);
    minina.setSource(SOURCE::Manual);
    minina.setImageUrl(std::string("https://example.com/granola.jpg"));
    meal.addFoodItem(minina.id(), 50);

    EXPECT_EQ(meal.food_items()[0].first, minina.id());
    EXPECT_EQ(meal.food_items()[0].second, 50);
}

TEST_F(MealModelTest, removeFoodItem) {
    cc::models::Food minina;
    minina.setId("2131654967498");
    minina.setName("minina");
    minina.setBrand(std::string("wlad 3icha l3arbi"));
    minina.setBarcode(std::string("2131654967498"));
    minina.setCaloriesPer100g(420.0);
    minina.setSource(SOURCE::Manual);
    minina.setImageUrl(std::string("https://example.com/granola.jpg"));
    meal.addFoodItem(minina.id(), 50);
    EXPECT_EQ(meal.food_items()[0].first, minina.id());
    EXPECT_EQ(meal.food_items()[0].second, 50);

    EXPECT_EQ(meal.removeFoodItem(minina.id()), true);
    EXPECT_EQ(meal.food_items().size(), 0);
    EXPECT_EQ(meal.removeFoodItem(minina.id()), false);
}

TEST_F(MealModelTest, setFoodItems) {
    // food item 1
    cc::models::Food minina;
    minina.setId("2131654967498");
    minina.setName("minina");
    minina.setBrand(std::string("wlad 3icha l3arbi"));
    minina.setBarcode(std::string("2131654967498"));
    minina.setCaloriesPer100g(420.0);
    minina.setSource(SOURCE::Manual);
    minina.setImageUrl(std::string("https://example.com/granola.jpg"));

    // food item 1
    cc::models::Food minina_2;
    minina_2.setId("88877788888");
    minina_2.setName("minina_2");
    minina_2.setBrand(std::string("factory_xx"));
    minina_2.setBarcode(std::string("88877788888"));
    minina_2.setCaloriesPer100g(420.0);
    minina_2.setSource(SOURCE::Manual);
    minina_2.setImageUrl(std::string("https://example.com/granola.jpg"));

    std::vector<std::pair<std::string, double>> food_items{{minina.id(), 100}, {minina_2.id(), 50}};

    meal.setFoodItems(food_items);
    EXPECT_EQ(meal.food_items()[0].first, minina.id());
    EXPECT_EQ(meal.food_items()[0].second, 100);
    EXPECT_EQ(meal.food_items()[1].first, minina_2.id());
    EXPECT_EQ(meal.food_items()[1].second, 50);
}
TEST_F(MealModelTest, from_Json) {
    nlohmann::json meal_json;
    meal_json["name"] = "Breakfast";
    meal_json["id"] = 100;
    std::vector<std::pair<std::string, double>> food_items{{"002", 100}, {"009", 50}};
    meal_json["foodItems"] = food_items;
    meal_json["tsUtc"] = cc::utils::toIso8601(std::chrono::system_clock::now());
    MealLog meal_lunch = meal_json;
    EXPECT_EQ(meal_json["name"].get<std::string>(), magic_enum::enum_name(meal_lunch.getName()));
    EXPECT_EQ(magic_enum::enum_name(meal_lunch.getName()), meal_json["name"].get<std::string>());
    EXPECT_EQ(meal_json["foodItems"][0][0].get<std::string>(), meal_lunch.food_items()[0].first);
    EXPECT_EQ(meal_json["foodItems"][0][1].get<double>(), meal_lunch.food_items()[0].second);
    EXPECT_EQ(meal_json["tsUtc"].get<std::string>(), cc::utils::toIso8601(meal_lunch.gettime()));
}

TEST_F(MealModelTest, to_Json) {
    meal.setName(MEALNAME::Breakfast);
    meal.setId(900);
    std::vector<std::pair<std::string, double>> food_items{{"002", 100}, {"009", 50}};
    meal.setFoodItems(food_items);
    meal.setTime(std::chrono::system_clock::now());
    ///////////////////////////////////
    nlohmann::json meal_json = meal;
    ///////////////////////////////////
    EXPECT_EQ(meal_json["name"].get<std::string>(), magic_enum::enum_name(meal.getName()));
    EXPECT_EQ(meal_json["id"].get<int>(), meal.id());
    EXPECT_EQ(meal_json["foodItems"][0][0].get<std::string>(), meal.food_items()[0].first);
    EXPECT_EQ(meal_json["foodItems"][0][1].get<double>(), meal.food_items()[0].second);
    EXPECT_EQ(meal_json["tsUtc"].get<std::string>(), cc::utils::toIso8601(meal.gettime()));
}
