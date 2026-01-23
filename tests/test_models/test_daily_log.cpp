#include <chrono>
#include <gtest/gtest.h>
#include <string>
#include <utility>
#include <vector>
#include "models/daily_log.hpp"
#include "models/meal_log.hpp"
#include "models/food.hpp"

using namespace cc::models;

class DailyLogModelTest : public ::testing::Test {
protected:
    void SetUp() override {                // runs BEFORE each TEST_F
                                                                           //
    }

    void TearDown() override {             // runs AFTER each TEST_F
           // nothing to destroy                                //
    }

    // helper functions and members visible to all TEST_F in this suite
};

TEST_F(DailyLogModelTest,initializition) {
    std::chrono::system_clock::time_point tsUtc = std::chrono::system_clock::now();
    DailyLog daily_log(tsUtc);
    EXPECT_EQ(daily_log.getDate(),tsUtc);
    }
TEST_F(DailyLogModelTest,addMeal) {
    DailyLog daily_log(std::chrono::system_clock::now());
    MEALNAME meal_name = MEALNAME::Lunch;
    MealLog meal(meal_name);
    daily_log.addMeal(meal);
    EXPECT_EQ(daily_log.meals()[0].getName(),meal_name);
    }
TEST_F(DailyLogModelTest,addMeals) {
    DailyLog daily_log(std::chrono::system_clock::now());
    MealLog lunch(MEALNAME::Lunch );
    MealLog breakfast(MEALNAME::Breakfast );
    std::vector<MealLog> meals{lunch,breakfast};
    daily_log.addMeals(meals);
    EXPECT_EQ(daily_log.meals()[0].getName(),MEALNAME::Lunch);
    EXPECT_EQ(daily_log.meals()[1].getName(),MEALNAME::Breakfast);
    }
TEST_F(DailyLogModelTest,setDate) {
    DailyLog daily_log(std::chrono::system_clock::now());
    std::chrono::system_clock::time_point tsUtc = std::chrono::system_clock::now();
    daily_log.setDate(tsUtc);
    EXPECT_EQ(daily_log.getDate(),tsUtc);
    }
